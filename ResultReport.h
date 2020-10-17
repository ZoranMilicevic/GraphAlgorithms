#pragma once
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <ctime>
#include "NodeResultReport.h"

class ServerCommand;

class ResultReport
{
public:
	ResultReport() {}
	virtual ~ResultReport() {};

	void add_node_result_report(const std::shared_ptr<NodeResultReport>& new_report)
	{
		//do not use mutex because each thread is writing to diffrent location in vector
		node_results[new_report->node_key] = new_report;
	}
	
	std::string to_string() const 
	{
		static const unsigned BUF_SIZE = 100;
		static const char* FORMAT = "%H:%M:%S";
		std::ostringstream res_string;

		std::time_t start = std::chrono::system_clock::to_time_t(start_time);
		std::time_t end = std::chrono::system_clock::to_time_t(end_time);
		char buf_start_time[BUF_SIZE] = { 0 };
		char buf_end_time[BUF_SIZE] = { 0 };

		std::strftime(buf_start_time, BUF_SIZE, FORMAT, std::localtime(&start));
		std::strftime(buf_end_time, BUF_SIZE, FORMAT, std::localtime(&end));

		res_string << "Start time: " << buf_start_time << std::endl;
		res_string << "End time: " << buf_end_time << std::endl;

		auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
		auto milis = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
		auto sec = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
		res_string << "Elapsed time: " << micros << "[us] = " << milis << "[ms] = " << sec << "[s]" << std::endl;

		for (auto&& node_result : node_results)
		{
			std::time_t time_visited = std::chrono::system_clock::to_time_t(node_result->time_visited);
			char buf_time_visited[BUF_SIZE] = { 0 };
			std::strftime(buf_time_visited, BUF_SIZE, FORMAT, std::localtime(&time_visited));

			res_string << "Node: " << node_result->node_key << std::endl;
			res_string << "Visitor thread ID: " << node_result->visitor_id << std::endl;
			res_string << "Time visited: " << buf_time_visited << std::endl;
		}

		return res_string.str();
	}
	


	std::chrono::system_clock::time_point start_time;
	std::chrono::system_clock::time_point end_time;
	std::vector<std::shared_ptr<NodeResultReport>> node_results;
};