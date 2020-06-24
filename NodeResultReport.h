#pragma once

#include <chrono>
#include <thread>

class NodeResultReport
{
public:
	NodeResultReport(std::thread::id visitor_id, std::chrono::system_clock::time_point time_visited)
		:visitor_id(visitor_id), time_visited(time_visited)
	{}

private:
	std::thread::id visitor_id;
	std::chrono::system_clock::time_point time_visited;
};
