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

	void execute_command();
	void execute_bfs_single_command();
	void execute_dfs_single_command();
	void execute_bfs_cpp_command();
	void execute_dfs_cpp_command();

	void create_graph()
	{
		this->graph_root = GraphGenerator::generate_graph(node_keys, edges, root_key);
	}

	GraphAlgorithm algorithm;
	int number_of_threads;
	int number_of_nodes;
	int node_traverse_time;
	int cond_var_wait_time;
	int root_key;
	std::string graph_type;

	std::vector<int> node_keys;
	std::vector<std::pair<int, int>> edges;
	GraphNode* graph_root;
};