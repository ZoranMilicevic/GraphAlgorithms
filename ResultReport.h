#pragma once
#include <chrono>
#include <vector>
#include <string>
#include "NodeResultReport.h"

class ServerCommand;

class ResultReport
{
public:
	static ResultReport* create_new_repport()
	{
		delete ResultReport::cur_repport;
		ResultReport::cur_repport = new ResultReport();
		return ResultReport::cur_repport;
	}

	static ResultReport* get_cur_repport()
	{
		if (cur_repport == NULL)
			create_new_repport();
		return cur_repport;
	}

	void fill_from_server_command(ServerCommand* sc);

	void add_node_result_report(NodeResultReport* new_report)
	{
		node_results[new_report->node_key] = new_report;
	}
	
	std::string to_xml_string();
	
	virtual ~ResultReport() 
	{
		for(int i = 0; i<number_of_nodes; i++)
		{
			delete node_results[i];
		}
		delete node_results;
	};

	std::string algorithm;
	int number_of_worker_threads;
	int number_of_nodes;
	int traverse_time;
	int wait_time;
	int root_key;
	std::string graph_type;

	std::chrono::system_clock::time_point start_time;
	std::chrono::system_clock::time_point end_time;
	std::chrono::duration<double> elapsed_time;
	NodeResultReport** node_results;

private:
	ResultReport() {};
	inline static ResultReport* cur_repport;
};