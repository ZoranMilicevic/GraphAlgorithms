#pragma once

#include <memory>
#include "ServerCommand.h"
#include "VisitedArrayCppThreads.h"

class BFS
{
public:
	static void BFS_ST(const std::shared_ptr<ServerCommand>& command);

	static void BFS_MT(const std::shared_ptr<ServerCommand>& command);
	static void BFS_MT_traversal(const std::shared_ptr<GraphNode>& node, std::shared_ptr<VisitedArrayCppThreads> visited, const std::shared_ptr<ServerCommand>& command);
};

