#pragma once

#include <chrono>

class NodeResultReport
{
private:
	int visitor_id;
	std::chrono::system_clock::time_point time_visited;
};
