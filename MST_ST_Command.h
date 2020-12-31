#pragma once

#include "ServerCommand.h"

class MST_ST_Command : public ServerCommand
{
public:
	MST_ST_Command(const std::string& graph_str, unsigned root_key, unsigned node_traverse_time, bool include_node_reports)
		: ServerCommand(graph_str, root_key, node_traverse_time, include_node_reports), mst_total_weight(0){}

	virtual void do_command() override;
	virtual std::string get_result_string() override;

private:
	unsigned mst_total_weight;
	std::vector<std::shared_ptr<GraphEdge>> mst;
};

