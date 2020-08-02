#pragma once
#include <chrono>
#include <vector>
#include <string>
#include <memory>
#include "NodeResultReport.h"

class ServerCommand;

class ResultReport
{
public:
	ResultReport(): elapsed_time(0) {}

	void add_node_result_report(const std::shared_ptr<NodeResultReport>& new_report)
	{
		//do not use mutex because each thread is writing to diffrent location in vector
		node_results[new_report->node_key] = new_report;
	}
	
	std::string to_xml_string(bool include_node_reports = false) const;
	
	virtual ~ResultReport() {};

	std::chrono::system_clock::time_point start_time;
	std::chrono::system_clock::time_point end_time;
	std::chrono::duration<double> elapsed_time;
	std::vector<std::shared_ptr<NodeResultReport>> node_results;
};