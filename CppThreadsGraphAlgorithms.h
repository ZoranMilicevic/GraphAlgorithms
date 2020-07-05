#pragma once

#include "GraphNode.h"
#include "VisitedArrayCppThreads.h"
#include "ServerCommand.h"
#include <memory>


class CppThreadsGraphAlgorithms{
public:
	static void DFS(const std::shared_ptr<ServerCommand>& command);
	static void dfs_traversal(const std::shared_ptr<GraphNode>& node, VisitedArrayCppThreads* visited);
	
	static void BFS(const std::shared_ptr<ServerCommand>& command);
	static void bfs_traversal(const std::shared_ptr<GraphNode>& node, VisitedArrayCppThreads* visited, const std::shared_ptr<ServerCommand>& sc);
};



