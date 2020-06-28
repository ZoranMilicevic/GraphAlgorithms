#pragma once
#include <vector>
#include <thread>
#include <chrono>
#include "NodeResultReport.h"
#include "ResultReport.h"

#include <iostream>

class GraphNode
{
public:
	int key;
	std::vector<GraphNode*> neighbours;

	GraphNode(int key) : key(key) {}
	GraphNode(int key, const std::vector<GraphNode*> neighbours) :key(key), neighbours(neighbours) {}
	~GraphNode(){
		for (GraphNode* elem : neighbours)
			delete elem;
	}


	void addNeighbour(GraphNode* node)
	{
		neighbours.push_back(node);
	}

	void traverseNode(int traverse_time) 
	{
		NodeResultReport* node_result_report = new NodeResultReport(key, std::this_thread::get_id(), std::chrono::system_clock::now());
		ResultReport::get_cur_repport()->add_node_result_report(node_result_report);
		
		std::chrono::microseconds node_traverse_time{ traverse_time };
		std::this_thread::sleep_for(node_traverse_time);
	}
};
