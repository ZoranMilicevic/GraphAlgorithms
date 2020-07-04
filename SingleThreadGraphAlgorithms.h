#pragma once

#include "ServerCommand.h"

class SingleThreadGraphAlgorithms
{
public:
	static void DFS(ServerCommand* command);
	static void BFS(ServerCommand* command);
};