#pragma once

#include "ServerCommand.h"
#include "MST_Tree.h"

class MST_ST_Command : public ServerCommand
{
public:
	MST_ST_Command(const std::string& graph_str, unsigned root_key, unsigned node_traverse_time, bool include_node_reports)
		: ServerCommand(graph_str, root_key, node_traverse_time, include_node_reports){}

	virtual void do_command() override;
	virtual std::string get_result_string() override;

private:
	MST_Tree mst;
};

