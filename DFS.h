#pragma once

#include <memory>
#include "ServerCommand.h"
#include "VisitedArrayCppThreads.h"
#include "DataStackSingleThread.h"
#include "DFS_MT_Util.h"
#include "StackWithSplit.h"

class DFS
{
public:
	static void DFS_ST(const std::shared_ptr<ServerCommand>& command);
	static void DFS_MT(const std::shared_ptr<ServerCommand>& command);

	static void DFS_MT_traversal(
		std::shared_ptr<DataStackSingleThread<GraphNode>> starting_stack,
		std::shared_ptr<VisitedArrayCppThreads> visited, 
		const std::shared_ptr<ServerCommand>& command,
		const std::shared_ptr<DFS_MT_Util>& util_struct
	);
};

