#pragma once

#include "ServerCommand.h"

class DFS_ST_Command : public ServerCommand
{
public:
	DFS_ST_Command(const std::string& graph_str, unsigned root_key, unsigned node_traverse_time, bool include_node_reports)
		: ServerCommand(graph_str, root_key, node_traverse_time, include_node_reports) {}

	virtual void do_command() override;
};

