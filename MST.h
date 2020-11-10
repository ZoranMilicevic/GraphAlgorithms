#pragma once
#include "ServerCommand.h"

class MST 
{
	static void MST_Kruskal(const std::shared_ptr<ServerCommand>& command);
	static void MST_Prim(const std::shared_ptr<ServerCommand>& command);
	static void MST_MT(const std::shared_ptr<ServerCommand>& command);
};