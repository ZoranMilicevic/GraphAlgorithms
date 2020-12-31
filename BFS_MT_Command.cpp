#include "BFS_MT_Command.h"

void BFS_MT_Command::do_command()
{
	result_report.start_time = std::chrono::system_clock::now();
	pool->submit([&]() -> void { this->BFS_MT_traversal(graph.root_node); });
	sem.wait();
}

void BFS_MT_Command::BFS_MT_traversal(std::shared_ptr<GraphNode> node)
{
	if (!visited->test_and_set_visited(node->key))
	{
		node->traverseNode(*this);
		visited->increment_visited();

		if (!visited->added_all()) {
			for (auto&& edge : node->outgoingEdges)
			{
				if (!visited->test_and_set_added(edge->node2->key))
				{
					pool->submit([&]() -> void { this->BFS_MT_traversal(edge->node2); });
					visited->increment_added();
				}
			}
		}
	}

	if (visited->visited_all())
	{
		if (!visited->test_and_set_end_time_writen())
		{
			result_report.end_time = std::chrono::system_clock::now();
			sem.post();
		}
	}
}
