#pragma once
#include <vector>
#include <thread>
#include <chrono>
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
		std::string ss = "LOGGER: Traversed node " + std::to_string(key);// + " by thread with id: " + std::to_string(std::this_thread::get_id()) + std::endl;
		Logger::getInstance()->logMessage(ss);
		std::cout << "Traversed node " << std::to_string(key) << " by thread with id: " << std::this_thread::get_id() << std::endl;
		std::this_thread::sleep_for(ConfigurationParameters::node_traverse_time);
	}
};
