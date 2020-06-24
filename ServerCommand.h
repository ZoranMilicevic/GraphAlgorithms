#pragma once

#include <map>
#include <string>
#include <utility>
#include "GraphAlgorithms.h"
#include "GraphNode.h"
#include "GraphGenerator.h"

class ServerCommand
{
public:
	static ServerCommand* create_from_xml(const char* buffer);

	ServerCommand() {};

	ServerCommand(
		GraphAlgorithm algorithm, 
		int number_of_threads, 
		GraphNode* root, 
		int number_of_nodes, 
		int node_traverse_time
	):algorithm(algorithm), 
		number_of_threads(number_of_threads),
		root(root),
		number_of_nodes(number_of_nodes),
		node_traverse_time(node_traverse_time)
	{}

	ResultReport* execute_command();
	ResultReport* execute_bfs_single_command();
	ResultReport* execute_dfs_single_command();
	ResultReport* execute_bfs_cpp_command();
	ResultReport* execute_dfs_cpp_command();

	void set_algorithm(GraphAlgorithm algorithm)
	{
		this->algorithm = algorithm;
	}

	void set_number_of_threads(int number_of_threads)
	{
		this->number_of_threads = number_of_threads;
	}

	void set_number_of_nodes(int number_of_nodes)
	{
		this->number_of_nodes = number_of_nodes;
	}

	void set_node_traverse_time(int node_traverse_time)
	{
		this->node_traverse_time = node_traverse_time;
	}

	void set_root_key(int root_key)
	{
		this->root_key = root_key;
	}

	void set_graph_type(std::string graph_type)
	{
		this->graph_type = graph_type;
	}

	void add_key(int key)
	{
		this->node_keys.push_back(key);
	}

	void add_edge(std::pair<int, int> edge)
	{
		this->edges.push_back(edge);
	}

	void create_graph()
	{
		this->root = GraphGenerator::generate_graph(node_keys, edges, root_key);
	}

private:
	GraphAlgorithm algorithm;
	int number_of_threads;
	int number_of_nodes;
	int node_traverse_time;
	int root_key;
	std::string graph_type;
	std::vector<int> node_keys;
	std::vector<std::pair<int, int>> edges;
	GraphNode* root;
};