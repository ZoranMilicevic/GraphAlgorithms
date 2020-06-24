#pragma once
#include <chrono>
#include <vector>
#include <string>
#include "NodeResultReport.h"
#include "DataQueueCppThreads.h"

class ResultReport
{
public:
	static ResultReport* get_cur_repport()
	{
		return cur_repport;
	}

	virtual void add_node_result_report(NodeResultReport* node_result_report) = 0;

protected:
	static ResultReport* cur_repport;

	std::chrono::system_clock::time_point start_time;
	std::chrono::system_clock::time_point end_time;
	int number_of_worker_threads;
	int number_of_nodes;
	int traverse_time;
	int wait_time;
	std::string graph_type;
};