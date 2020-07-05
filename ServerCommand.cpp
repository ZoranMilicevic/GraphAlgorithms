#include <string>
#include <sstream>
#include "pugixml.hpp"
#include "SingleThreadGraphAlgorithms.h"
#include "CppThreadsGraphAlgorithms.h"
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
	shared_ptr<ServerCommand> sh_sc(this);
	switch (this->algorithm)
	{
	case GraphAlgorithm::BFS_SINGLE:
		SingleThreadGraphAlgorithms::BFS(sh_sc);
		break;
	case GraphAlgorithm::DFS_SINGLE:
		SingleThreadGraphAlgorithms::DFS(sh_sc);
		break;
	case GraphAlgorithm::BFS_CPP:
		CppThreadsGraphAlgorithms::BFS(sh_sc);
		break;
	case GraphAlgorithm::DFS_CPP:
		CppThreadsGraphAlgorithms::DFS(sh_sc);
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



