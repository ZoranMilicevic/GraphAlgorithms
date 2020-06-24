#pragma once

#include "GraphNode.h"
#include "ThreadPool.h"
#include "VisitedArrayCppThreads.h"


class CppThreadsGraphAlgorithms{
public:
	static void DFS(GraphNode* node, int number_of_nodes);
	static void dfs_traversal(GraphNode* node, VisitedArrayCppThreads* visited);
	
	static void BFS(GraphNode* node, int number_of_nodes);
	static void bfs_traversal(GraphNode* node, VisitedArrayCppThreads* visited);
};



