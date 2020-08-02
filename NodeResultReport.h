#pragma once

#include <chrono>
#include <thread>

class NodeResultReport
{
public:
	NodeResultReport(int node_key, std::thread::id visitor_id, std::chrono::system_clock::time_point time_visited)
		:node_key(node_key), visitor_id(visitor_id), time_visited(time_visited)
	{}

	int node_key;
	std::thread::id visitor_id;
	std::chrono::system_clock::time_point time_visited;
};
