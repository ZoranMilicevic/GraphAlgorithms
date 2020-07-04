#pragma once

#include <map>
#include <string>
#include <utility>
#include "ResultReport.h"
#include "GraphNode.h"

enum class GraphAlgorithm;

class ServerCommand
{
public:
	static ServerCommand* create_from_xml(const std::string& buffer);

	ServerCommand() {};
	~ServerCommand()
	{
		delete graph_root;
	}

	void execute_command();

	std::string result_xml()const;

	void create_graph();

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

	ResultReport result_report;
};