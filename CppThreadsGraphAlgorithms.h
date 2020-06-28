#pragma once

#include "GraphNode.h"
#include "ThreadPool.h"
#include "VisitedArrayCppThreads.h"
#include "ServerCommand.h"


class CppThreadsGraphAlgorithms{
public:
	static void DFS(ServerCommand* command);
	static void dfs_traversal(GraphNode* node, VisitedArrayCppThreads* visited);
	
	static void BFS(ServerCommand* command);
	static void bfs_traversal(GraphNode* node, VisitedArrayCppThreads* visited, int traverse_time);
};



