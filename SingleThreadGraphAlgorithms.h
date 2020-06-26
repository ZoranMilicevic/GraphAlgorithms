#pragma once

#include "GraphNode.h"
#include "VisitedArraySingleThread.h"

class SingleThreadGraphAlgorithms
{
public:
	static void DFS(GraphNode* root, int number_of_nodes);
	static void BFS(GraphNode* root, int number_of_nodes);
};