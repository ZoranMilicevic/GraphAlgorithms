#pragma once

#include "GraphNode.h"
#include "VisitedArraySingleThread.h"

class SingleThreadGraphAlgorithms
{
public:
	SingleThreadGraphAlgorithms(GraphNode* root, int number_of_nodes)
		:root(root), number_of_nodes(number_of_nodes), visited(new VisitedArraySingleThread(number_of_nodes)){};
	virtual ~SingleThreadGraphAlgorithms()
	{
		delete visited;
	}

	virtual void DFS();
	virtual void BFS();

protected:
	GraphNode* root;
	int number_of_nodes;
	VisitedArraySingleThread* visited;
};