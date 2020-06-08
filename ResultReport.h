#pragma once

#include <chrono>
#include "GraphType.h"

class ResultReport
{
public:
private:
	std::chrono::system_clock::time_point start_time;
	std::chrono::system_clock::time_point end_time;
	int number_of_worker_threads;
	int number_of_nodes;
	int traverse_time;
	int wait_time;
	GraphType graph_type;

};