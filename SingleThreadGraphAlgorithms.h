#pragma once
#include <memory>
#include "ServerCommand.h"

class SingleThreadGraphAlgorithms
{
public:
	static void DFS(const std::shared_ptr<ServerCommand>& command);
	static void BFS(const std::shared_ptr<ServerCommand>& command);
};