#include "DFS_ST_Command.h"
#include "VisitedArray.h"
#include "DataStack.h"

void DFS_ST_Command::do_command()
{
	result_report.start_time = std::chrono::system_clock::now();

	DataStack<GraphNode> stack;
	VisitedArray visited(graph.all_nodes.size());

	stack.push(*graph.root_node);

	while (!stack.empty())
	{
		std::shared_ptr<GraphNode> curNode = stack.pop();

		if (!visited.test_and_set_visited(curNode->key))
		{
			curNode->traverseNode(*this);
			if (visited.increment_visited())
				break;
		}

		for (auto&& edge : curNode->outgoingEdges) {
			if (!visited.is_visited(edge->node2->key))
				stack.push(*edge->node2);
		}
	}

	result_report.end_time = std::chrono::system_clock::now();
}
