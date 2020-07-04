#pragma once
#include <vector>
#include <thread>
#include <chrono>
#include "NodeResultReport.h"

class ServerCommand;

class GraphNode
{
public:
	int key;
	std::vector<GraphNode*> neighbours;

	GraphNode(int key) : key(key) {}
	GraphNode(int key, const std::vector<GraphNode*> neighbours) :key(key), neighbours(neighbours) {}
	~GraphNode(){
		//for (GraphNode* elem : neighbours)
			//delete elem;
	}


	void addNeighbour(GraphNode* node)
	{
		neighbours.push_back(node);
	}

	void traverseNode(ServerCommand* command);
};
