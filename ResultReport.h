#pragma once
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <ctime>
#include <chrono>
#include <thread>

class ServerCommand;

class NodeResultReport
{
public:
	NodeResultReport(int node_key, std::thread::id visitor_id, std::chrono::system_clock::time_point time_visited)
		:node_key(node_key), visitor_id(visitor_id), time_visited(time_visited) {}

	int node_key;
	std::thread::id visitor_id;
	std::chrono::system_clock::time_point time_visited;
};


class ResultReport
{
public:
	ResultReport() {}

	void add_node_result_report(const std::shared_ptr<NodeResultReport>& new_report)
	{
		//do not use mutex because each thread is writing to diffrent location in vector
		node_results[new_report->node_key] = new_report;
	}
	
	std::string to_string() const;
	
	std::chrono::system_clock::time_point start_time;
	std::chrono::system_clock::time_point end_time;
	std::vector<std::shared_ptr<NodeResultReport>> node_results;
};