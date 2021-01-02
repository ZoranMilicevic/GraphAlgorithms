#include "DFS_ST_Command.h"
#include "VisitedArray.h"
#include <vector>

void DFS_ST_Command::do_command()
{
	result_report.start_time = std::chrono::system_clock::now();

	std::vector<std::shared_ptr<GraphNode>> stack;
	VisitedArray visited(graph.all_nodes.size());

	stack.push_back(graph.root_node);

	while (!stack.empty())
	{
		std::shared_ptr<GraphNode> curNode = stack.back();
		stack.pop_back();

		if (!visited.test_and_set_visited(curNode->key))
		{
			curNode->traverseNode(node_traverse_time);
			if (include_node_reports)
				result_report.add_node_result_report(NodeResultReport(curNode->key, std::this_thread::get_id(), std::chrono::system_clock::now()));

			if (visited.increment_visited())
				break;
		}

		for (auto&& edge : curNode->outgoingEdges) {
			if (!visited.is_visited(edge->node2->key))
				stack.push_back(edge->node2);
		}
	}

	result_report.end_time = std::chrono::system_clock::now();
}
