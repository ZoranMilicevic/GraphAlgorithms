#include <conio.h>
#include <Python.h>
#include <filesystem>
#include <vector>
#include <iostream>
#include <utility>

#include "CPyInstance.h"
#include "CPyObject.h"
#include "GraphGenerator.h"

using namespace std;
using namespace std::filesystem;

GraphGenerator::GraphGenerator()
{
	CPyObject moduleName = PyUnicode_FromString(ConfigurationParameters::py_module_name.c_str());
	pModule = PyImport_Import(moduleName);

	if (!pModule)
	{
		Logger::get_instance()->log_message("Could not find python function!");
		exit(1);
	}
}

GraphGenerator& GraphGenerator::get_instance()
{
	static GraphGenerator instance;
	return instance;
}

GraphNode* GraphGenerator::generate_graph(const GraphType type, int nodes)
{
	string py_meth_name = ConfigurationParameters::py_method_names.at(type);

	CPyObject pFunc = PyObject_GetAttrString(pModule, py_meth_name.c_str());
	if (pFunc && PyCallable_Check(pFunc))
	{
		CPyObject py_graph = PyObject_CallObject(pFunc, NULL);
		return create_cpp_graph_from_py_graph(py_graph);
	}
	else 
	{
		Logger::get_instance()->log_message("Could not find python function!");
		exit(1);
	}

	return new GraphNode(1);
}

GraphNode* GraphGenerator::create_cpp_graph_from_py_graph(CPyObject py_graph)
{
	CPyObject nodesPyList = PyTuple_GetItem(py_graph, 0);
	vector<int> nodes_int;

	ssize_t nodes_list_len = (ssize_t)PyList_GET_SIZE(nodesPyList.getObject());
	for (size_t i = 0; i < nodes_list_len; i++)
	{
		CPyObject elem = PyList_GetItem(nodesPyList, i);
		nodes_int.push_back(PyLong_AsLongLong(elem));
	}

	CPyObject edgesPyList = PyTuple_GetItem(py_graph, 1);
	vector<pair<int, int>> edges_int;

	ssize_t edges_list_len = (ssize_t)PyList_GET_SIZE(edgesPyList.getObject());
	for (ssize_t i = 0; i < edges_list_len; i++)
	{
		CPyObject elem = PyList_GetItem(edgesPyList, i);
		CPyObject val1 = PyTuple_GetItem(elem, 0);
		CPyObject val2 = PyTuple_GetItem(elem, 1);
		int val1_int = (int)PyLong_AsLongLong(val1);
		int val2_int = (int)PyLong_AsLongLong(val2);

		edges_int.push_back(pair<int, int>(val1_int, val2_int));
	}

	return create_cpp_graph_from_nodes_and_edges(nodes_int, edges_int);
}

GraphNode* GraphGenerator::create_cpp_graph_from_nodes_and_edges(const vector<int>& node_keys, const vector<std::pair<int, int>>& edges)
{
	vector<GraphNode*> nodes;
	for(auto&& key : node_keys)
	{
		nodes.push_back(new GraphNode(key));
	}

	for(auto&& elem : edges)
	{
		int node1_key = elem.first;
		int node2_key = elem.second;

		nodes[node1_key]->addNeighbour(nodes[node2_key]);
		nodes[node2_key]->addNeighbour(nodes[node1_key]);
	}

	return nodes[0];
}




