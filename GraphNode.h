#pragma once
#include <vector>
#include <thread>
#include <chrono>
#include <sstream>
#include "Logger.h"
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
		std::ostringstream ss;
		ss << "INFO: Traversed node " << key << " by thread with id: " << std::this_thread::get_id();
		Logger::get_instance()->log_message(ss.str());
		std::this_thread::sleep_for(ConfigurationParameters::node_traverse_time);
	}
};
