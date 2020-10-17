#include "GraphNode.h"
#include "ServerCommand.h"

void GraphNode::traverseNode(const std::shared_ptr<ServerCommand>& command)
{
	if (command->include_node_reports) 
	{
		std::shared_ptr<NodeResultReport> node_res_rep(new NodeResultReport(key, std::this_thread::get_id(), std::chrono::system_clock::now()));
		command->result_report.add_node_result_report(node_res_rep);
	}

	if (command->node_traverse_time > 0) 
	{
		std::chrono::microseconds node_traverse_time{ command->node_traverse_time };
		std::this_thread::sleep_for(node_traverse_time);
	}
}
