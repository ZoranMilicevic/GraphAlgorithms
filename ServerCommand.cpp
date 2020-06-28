#include <string>
#include "ServerCommand.h"
#include "pugixml.hpp"
#include "SingleThreadGraphAlgorithms.h"
#include "CppThreadsGraphAlgorithms.h"
#include "GraphAlgorithms.h"

using namespace std;
using namespace pugi;


void process_attribute(const char* name, const char* value, ServerCommand* sc)
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


ServerCommand* ServerCommand::create_from_xml(const string& buffer)
{
	xml_document doc;
	ServerCommand* sc = new ServerCommand();

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
	case GraphAlgorithm::BFS_SINGLE:
		this->execute_bfs_single_command();
		break;
	case GraphAlgorithm::DFS_SINGLE:
		this->execute_dfs_single_command();
		break;
	case GraphAlgorithm::BFS_CPP:
		this->execute_bfs_cpp_command();
		break;
	case GraphAlgorithm::DFS_CPP:
		this->execute_dfs_cpp_command();
		break;
	default:
		break;
	}

}

void ServerCommand::execute_bfs_single_command()
{
	ResultReport::create_new_repport()->fill_from_server_command(this);
	SingleThreadGraphAlgorithms::BFS(this->graph_root, this->number_of_nodes);
}

void ServerCommand::execute_dfs_single_command()
{
	ResultReport::create_new_repport()->fill_from_server_command(this);
	SingleThreadGraphAlgorithms::DFS(this->graph_root, this->number_of_nodes);
}

void ServerCommand::execute_bfs_cpp_command()
{
	ResultReport::create_new_repport()->fill_from_server_command(this);
}

void ServerCommand::execute_dfs_cpp_command()
{
	ResultReport::create_new_repport()->fill_from_server_command(this);
}





