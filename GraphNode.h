#pragma once
#include <vector>
#include <thread>
#include <chrono>
#include "NodeResultReport.h"
#include "ResultReport.h"
#include "ConfigurationParameters.h"

#include <iostream>

class GraphNode
{
public:
	int key;
	std::vector<GraphNode*> neighbours;

	GraphNode(int key) : key(key) {}
	GraphNode(int key, const std::vector<GraphNode*> neighbours) :key(key), neighbours(neighbours) {}

	void addNeighbour(GraphNode* node)
	{
		neighbours.push_back(node);
	}

	void traverseNode() 
	{
		NodeResultReport* node_result_report = new NodeResultReport(key, std::this_thread::get_id(), std::chrono::system_clock::now());
		ResultReport::get_cur_repport()->add_node_result_report(node_result_report);
		std::this_thread::sleep_for(ConfigurationParameters::node_traverse_time);
	}
};
