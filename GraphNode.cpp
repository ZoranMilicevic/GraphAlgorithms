#include "GraphNode.h"
#include "ServerCommand.h"
#include <iostream>

void GraphNode::traverseNode(const std::shared_ptr<ServerCommand>& command)
{
	command->result_report.add_node_result_report(NodeResultReport(key, std::this_thread::get_id(), std::chrono::system_clock::now()));

	std::chrono::microseconds node_traverse_time{ command->node_traverse_time };
	std::cout << key << " id:" <<std::this_thread::get_id() <<std::endl;
	std::this_thread::sleep_for(node_traverse_time);
}
