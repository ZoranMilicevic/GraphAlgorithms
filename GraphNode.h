#pragma once
#include <vector>
#include <thread>
#include <chrono>
#include <memory>
#include "NodeResultReport.h"

class ServerCommand;

class GraphNode
{
public:
	int key;
	std::vector<std::shared_ptr<GraphNode>> neighbours;

	GraphNode(int key) : key(key) {}
	GraphNode(int key, const std::vector<std::shared_ptr<GraphNode>> neighbours) :key(key), neighbours(neighbours) {}
	~GraphNode(){}


	void addNeighbour(std::shared_ptr<GraphNode> node)
	{
		neighbours.push_back(node);
	}

	void traverseNode(const std::shared_ptr<ServerCommand>& command);
};
