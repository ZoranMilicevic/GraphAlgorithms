#include "GraphNode.h"
#include "ServerCommand.h"

void GraphNode::traverseNode(ServerCommand* command)
{
	command->result_report.add_node_result_report(NodeResultReport(key, std::this_thread::get_id(), std::chrono::system_clock::now()));

	std::chrono::microseconds node_traverse_time{ command->node_traverse_time };
	std::this_thread::sleep_for(node_traverse_time);
}
