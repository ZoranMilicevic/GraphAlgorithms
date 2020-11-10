#pragma once

#include <map>
#include <string>
#include <utility>
#include <memory>
#include <thread>
#include "Semaphore.h"
#include "ResultReport.h"
#include "GraphNode.h"

enum class GraphAlgorithm;

class ServerCommand : public std::enable_shared_from_this<ServerCommand>
{
public:
	ServerCommand(
		unsigned number_of_threads, unsigned number_of_nodes, unsigned node_traverse_time, unsigned root_key,
		unsigned polling_param, unsigned sufficiency_param, bool include_node_reports, const std::string& graph_str, bool directed_graph
	) : number_of_nodes(number_of_nodes), node_traverse_time(node_traverse_time), root_key(root_key), 
		polling_param(polling_param), sufficiency_param(sufficiency_param),
		include_node_reports(include_node_reports), directed_graph(directed_graph), sem(0) 
	{
		if (number_of_threads == 0 || number_of_threads > std::thread::hardware_concurrency())
			this->number_of_threads = std::thread::hardware_concurrency();
		if (include_node_reports)
			result_report.node_results.resize(number_of_nodes);
		create_graph_from_string(graph_str);
	};
	~ServerCommand(){}

	void create_graph_from_string(const std::string& graph_str);

	//config stuff
	unsigned number_of_threads;
	unsigned number_of_nodes;
	unsigned node_traverse_time;
	unsigned root_key;
	unsigned polling_param;
	unsigned sufficiency_param;
	bool include_node_reports;
	bool directed_graph;

	//graph root
	std::shared_ptr<GraphNode> graph_root;

	//command result
	ResultReport result_report;
	
	FastSemaphore sem;
};