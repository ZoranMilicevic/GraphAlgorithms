#include <vector>
#include <memory>
#include <chrono>
#include "BFS_ST_Command.h"
#include "DataQueue.h"
#include "VisitedArray.h"

void BFS_ST_Command::do_command()
{
	result_report.start_time = std::chrono::system_clock::now();

	DataQueue<GraphNode> queue;
	VisitedArray visited(graph.all_nodes.size());

	queue.push(*graph.root_node);

	while (!queue.empty())
	{
		std::shared_ptr<GraphNode> curNode = queue.pop();

		if (!visited.test_and_set_visited(curNode->key))
		{
			curNode->traverseNode(*this);
			if (visited.increment_visited())
				break;
		}

		for (auto&& edge : curNode->outgoingEdges) {
			if (!visited.is_visited(edge->node2->key))
				queue.push(*edge->node2);
		}
	}

	result_report.end_time = std::chrono::system_clock::now();

}
