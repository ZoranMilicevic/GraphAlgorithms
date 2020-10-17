#pragma once

#include <map>
#include <string>
#include <utility>
#include <memory>
#include "Semaphore.h"
#include "ResultReport.h"
#include "GraphNode.h"

enum class GraphAlgorithm;

class ServerCommand : public std::enable_shared_from_this<ServerCommand>
{
public:
	static std::shared_ptr<ServerCommand> create_from_xml(const std::string& buffer);

	ServerCommand() : number_of_threads(0), number_of_nodes(0),
		node_traverse_time(0), root_key(0), graph_type(""), polling_param(0), sufficiency_param(0),
		include_node_reports(0), sem(0){};
	~ServerCommand(){}

	void execute_command();
	std::string result_xml()const;
	void create_graph();

	//config stuff
	GraphAlgorithm algorithm;
	int number_of_threads;
	int number_of_nodes;
	int node_traverse_time;
	int root_key;
	std::string graph_type;
	int polling_param;
	int sufficiency_param;
	bool include_node_reports;

	//graph stuff
	std::vector<int> node_keys;
	std::vector<std::pair<int, int>> edges;
	std::shared_ptr<GraphNode> graph_root;

	//command result
	ResultReport result_report;
	
	FastSemaphore sem;
};