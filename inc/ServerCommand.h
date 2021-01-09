#pragma once

#include <map>
#include <string>
#include <utility>
#include <memory>
#include <thread>
#include "Semaphore.h"
#include "ResultReport.h"
#include "Graph.h"

class ServerCommand
{
public:
	ServerCommand(const std::string& graph_str, unsigned root_key, unsigned node_traverse_time, bool include_node_reports)
		: graph(graph_str, root_key), node_traverse_time(node_traverse_time), include_node_reports(include_node_reports) {};

	virtual void do_command() = 0;
	virtual std::string get_result_string() 
	{
		return result_report.to_string();
	}

	//config stuff
	unsigned node_traverse_time;
	bool include_node_reports;

	//graph info
	Graph graph;

	//command result
	ResultReport result_report;
};