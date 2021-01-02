#include <vector>
#include <memory>
#include <chrono>
#include <deque>
#include "BFS_ST_Command.h"
#include "VisitedArray.h"

void BFS_ST_Command::do_command()
{
	result_report.start_time = std::chrono::system_clock::now();

	std::deque<std::shared_ptr<GraphNode>> queue;
	VisitedArray visited(graph.all_nodes.size());

	queue.push_front(graph.root_node);

	while (!queue.empty())
	{
		std::shared_ptr<GraphNode> curNode = queue.back();
		queue.pop_back();

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
				queue.push_front(edge->node2);
		}
	}

	result_report.end_time = std::chrono::system_clock::now();

}
