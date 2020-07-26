#pragma once

#include <map>
#include <string>
#include <utility>
#include <memory>
#include "ResultReport.h"
#include "GraphNode.h"

enum class GraphAlgorithm;

class ServerCommand : public std::enable_shared_from_this<ServerCommand>
{
public:
	static std::shared_ptr<ServerCommand> create_from_xml(const std::string& buffer);

	ServerCommand() {};
	~ServerCommand(){}

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
	int polling_param;
	int sufficiency_param;

	std::vector<int> node_keys;
	std::vector<std::pair<int, int>> edges;
	std::shared_ptr<GraphNode> graph_root;

	ResultReport result_report;
};