#include <string>
#include "GraphAlgorithms.h"
#include "ServerCommand.h"
#include "pugixml.hpp"

using namespace std;
using namespace pugi;


void process_attribute(const char* name, const char* value, ServerCommand* sc)
{
	static const string delimiter = "<->";
	string name_str(name);
	string value_str(value);

	if (name_str == "edge")
	{
		int pos = value_str.find(delimiter);
		string first_key_str = value_str.substr(0, pos);
		int first_key = stoi(first_key_str);
		value_str.erase(0, pos + delimiter.length());
		int second_key = stoi(value_str);

		sc->add_edge(make_pair(first_key, second_key));
	}
	else if (name_str == "node_key")
	{
		sc->add_key(stoi(value_str));
	}
	else if(name_str == "algorithm")
	{
		if (value_str == "dfs_single")
			sc->set_algorithm(GraphAlgorithm::DFS_SINGLE);
		else if(value_str == "bfs_single")
			sc->set_algorithm(GraphAlgorithm::BFS_SINGLE);
		else if(value_str == "dfs_cpp")
			sc->set_algorithm(GraphAlgorithm::DFS_CPP);
		else if(value_str == "bfs_cpp")
			sc->set_algorithm(GraphAlgorithm::BFS_CPP)
	}
	else if(name_str == "numberOfThreads")
	{
		sc->set_number_of_threads(stoi(value_str));
	}
	else if(name_str == "numberOfNodes")
	{
		sc->set_number_of_nodes(stoi(value_str));
	}
	else if(name_str == "nodeTraverseTime")
	{
		sc->set_node_traverse_time(stoi(value_str));
	}
	else if(name_str == "rootKey")
	{
		sc->set_root_key(stoi(value_str));
	}
	else if(name_str == "graphType")
	{
		sc->set_graph_type(value_str);
	}

}

ServerCommand* ServerCommand::create_from_xml(const char* buffer)
{
	xml_document doc;
	ServerCommand* sc = new ServerCommand();
	xml_parse_result res = doc.load_string((const pugi::char_t*)buffer);
	
	for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling())
	{
		for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute())
		{
			const char* name = attr.name();
			const char* value = attr.value();
			process_attribute(name, value, sc);
		}
	}

	sc->create_graph();
	
	return sc;
}

ResultReport* ServerCommand::execute_command()
{
	ResultReport* ret_val;

	switch (this->algorithm)
	{
	case GraphAlgorithm::BFS_SINGLE:

		break;
	case GraphAlgorithm::DFS_SINGLE:
		break;
	case GraphAlgorithm::BFS_CPP:
		break;
	case GraphAlgorithm::DFS_CPP:
		break;
	default:
		break;
	}


	return ret_val;
}


