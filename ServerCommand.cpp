#include <string>
#include <sstream>
#include "pugixml.hpp"
#include "DFS.h"
#include "BFS.h"
#include "GraphAlgorithms.h"
#include "GraphGenerator.h"


using namespace std;
using namespace pugi;


void process_attribute(const char* name, const char* value, const shared_ptr<ServerCommand>& sc)
{
	static const string delimiter = "<->";
	string name_str(name);
	string value_str(value);

	if(name_str == "algorithm")
	{
		sc->algorithm = string_to_graph_algorithm.at(value_str);
	}
	else if(name_str == "numberOfThreads")
	{
		sc->number_of_threads = stoi(value_str);
	}
	else if(name_str == "numberOfNodes")
	{
		sc->number_of_nodes = stoi(value_str);
		sc->result_report.node_results.resize(sc->number_of_nodes);
	}
	else if(name_str == "nodeTraverseTime")
	{
		sc->node_traverse_time = stoi(value_str);
	}
	else if (name_str == "condVarWaitTime")
	{
		sc->cond_var_wait_time = stoi(value_str);
	}
	else if(name_str == "rootKey")
	{
		sc->root_key = stoi(value_str);
	}
	else if(name_str == "graphType")
	{
		sc->graph_type = value_str;
	}
	else if (name_str == "polling_param") 
	{
		sc->polling_param = stoi(value_str);
	}
	else if(name_str == "sufficiency_param")
	{
		sc->sufficiency_param = stoi(value_str);
	}
}

shared_ptr<ServerCommand> ServerCommand::create_from_xml(const string& buffer)
{
	xml_document doc;
	shared_ptr<ServerCommand> sc = make_shared<ServerCommand>();

	xml_parse_result res = doc.load_string((const pugi::char_t*)buffer.c_str());
	pugi::xml_node params = doc.child("serverCommand").child("params");

	for (pugi::xml_attribute attr : params.attributes())
	{
		process_attribute(attr.name(), attr.value(), sc);
	}

	pugi::xml_node nodes = doc.child("serverCommand").child("graph").child("nodes");
	for(pugi::xml_node node : nodes.children("node"))
	{
		string value = node.attribute("nodeKey").value();
		sc->node_keys.push_back(stoi(value));
	}

	pugi::xml_node edges = doc.child("serverCommand").child("graph").child("edges");
	for (pugi::xml_node edge : edges.children("edge"))
	{
		string edge1 = edge.attribute("edge1").value();
		string edge2 = edge.attribute("edge2").value();
		int key1 = stoi(edge1);
		int key2 = stoi(edge2);
		sc->edges.push_back(make_pair(key1, key2));
	}

	sc->graph_root = GraphGenerator::generate_graph(sc->node_keys, sc->edges, sc->root_key);
	
	return sc;
}

void ServerCommand::execute_command()
{
	switch (this->algorithm)
	{
	case GraphAlgorithm::BFS_ST:
		BFS::BFS_ST(shared_from_this());
		break;
	case GraphAlgorithm::DFS_ST:
		DFS::DFS_ST(shared_from_this());
		break;
	case GraphAlgorithm::BFS_MT:
		BFS::BFS_MT(shared_from_this());
		break;
	case GraphAlgorithm::DFS_MT:
		DFS::DFS_MT(shared_from_this());
		break;
	default:
		break;
	}

}

std::string ServerCommand::result_xml() const
{
	std::ostringstream xml_string;
	xml_string << "<algorithm>" << graph_algorithm_to_string[(int)algorithm] << "</algorithm>";
	xml_string << "<numberOfThreads>" << number_of_threads << "</numberOfThreads>";
	xml_string << "<numberOfNodes>" << number_of_nodes << "</numberOfNodes>";
	xml_string << "<nodeTraverseTime>" << node_traverse_time << "</nodeTraverseTime>";
	xml_string << "<condVarWaitTime>" << cond_var_wait_time << "</condVarWaitTime>";
	xml_string << "<rootKey>" << root_key << "</rootKey>";
	xml_string << "<graphType>" << graph_type << "</graphType>";

	xml_string << result_report.to_xml_string();
	return xml_string.str();
}

void ServerCommand::create_graph()
{
	this->graph_root = GraphGenerator::generate_graph(node_keys, edges, root_key);
}



