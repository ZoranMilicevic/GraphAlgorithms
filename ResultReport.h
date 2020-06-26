#pragma once
#include <chrono>
#include <vector>
#include <string>
#include <sstream>
#include "NodeResultReport.h"
#include "ServerCommand.h"

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

	void fill_from_server_command(ServerCommand* sc)
	{
		this->algorithm = graph_algorithm_to_string(sc->algorithm);
		this->number_of_worker_threads = sc->number_of_threads;
		this->number_of_nodes = sc->number_of_nodes;
		this->traverse_time = sc->node_traverse_time;
		this->wait_time = sc->cond_var_wait_time;
		this->root_key = sc->root_key;
		this->graph_type = sc->graph_type;

		node_results = new NodeResultReport* [number_of_nodes];
	}

	void add_node_result_report(NodeResultReport* new_report)
	{
		node_results[new_report->node_key] = new_report;
	}

	std::string to_xml_string()
	{
		std::ostringstream xml_string;
		xml_string << "<algorithm>" << algorithm << "<\\algorithm>";
		xml_string << "<numberOfThreads>" << number_of_worker_threads << "<\\numberOfThreads>";
		xml_string << "<numberOfNodes>" << number_of_nodes << "<\\numberOfNodes>";
		xml_string << "<nodeTraverseTime>" << traverse_time << "<\\nodeTraverseTime>";
		xml_string << "<condVarWaitTime>" << wait_time << "<\\condVarWaitTime>";
		xml_string << "<rootKey>" << root_key << "<\\rootKey>";
		xml_string << "<graphType>" << graph_type << "<\\graphType>";

		//xml_string << "<startTime>" << start_time << "<\\startTime>";
		//xml_string << "<startTime>" << start_time << "<\\startTime>";
		//xml_string << "<elapsedTime>" << elapsed_time << "<\\elapsedTime>";
	}

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