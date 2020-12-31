#pragma once

#include "ServerCommand.h"

class MST_MT_Command : public ServerCommand
{
public:
	MST_MT_Command(const std::string& graph_str, unsigned root_key, unsigned node_traverse_time, bool include_node_reports,
		unsigned number_of_threads)
		: ServerCommand(graph_str, root_key, node_traverse_time, include_node_reports), number_of_threads(number_of_threads) {}

	virtual void do_command() override;

private:
	unsigned number_of_threads;
};

