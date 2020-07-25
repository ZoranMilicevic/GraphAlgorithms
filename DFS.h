#pragma once

#include <memory>
#include "ServerCommand.h"
#include "VisitedArrayCppThreads.h"

class DFS
{
public:
	static void DFS_ST(const std::shared_ptr<ServerCommand>& command);
	static void DFS_MT(const std::shared_ptr<ServerCommand>& command);
private:
	static void DFS_MT_traversal(const std::shared_ptr<GraphNode>& node, std::shared_ptr<VisitedArrayCppThreads> visited, const std::shared_ptr<ServerCommand>& command);
};

