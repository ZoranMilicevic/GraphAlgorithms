#include <vector>
#include <memory>
#include <chrono>
#include "BFS.h"
#include "ThreadPool.h"
#include "DataQueueSingleThread.h"
#include "GraphNode.h"
#include "VisitedArraySingleThread.h"

using namespace std;

void BFS::BFS_ST(const shared_ptr<ServerCommand>& command)
{
	command->result_report.start_time = chrono::system_clock::now();

	DataQueueSingleThread<GraphNode> queue;
	VisitedArraySingleThread visited(command->number_of_nodes);

	queue.push(*command->graph_root);

	while (!queue.empty())
	{
		std::shared_ptr<GraphNode> curNode = queue.pop();

		if (!visited.test_and_set_visited(curNode->key))
		{
			curNode->traverseNode(command);
			if (visited.increment_visited())
				break;
		}

		for (auto&& edge : curNode->outgoingEdges) {
			if (!visited.is_visited(edge->toNode->key))
				queue.push(*edge->toNode);
		}
	}

	command->result_report.end_time = chrono::system_clock::now();
}



void BFS::BFS_MT(const shared_ptr<ServerCommand>& command)
{
	shared_ptr<VisitedArrayCppThreads> visited(new VisitedArrayCppThreads(command->number_of_nodes));

	std::shared_ptr<ThreadPool> pool = std::make_shared<ThreadPool>(command->number_of_threads);
	command->result_report.start_time = chrono::system_clock::now();

	pool->submit(
		[=]()->void { BFS::BFS_MT_traversal(command->graph_root, visited, command, pool); }
	);

	command->sem.wait();
}

void BFS::BFS_MT_traversal(
	const shared_ptr<GraphNode>& node, 
	shared_ptr<VisitedArrayCppThreads> visited, 
	const shared_ptr<ServerCommand>& command, 
	std::shared_ptr<ThreadPool> pool
)
{
	if (!visited->test_and_set_visited(node->key))
	{
		node->traverseNode(command);
		visited->increment_visited();

		if (!visited->added_all()) {
			for (auto&& edge : node->outgoingEdges)
			{
				if (!visited->test_and_set_added(edge->toNode->key))
				{
					pool->submit([=]()->void { BFS::BFS_MT_traversal(edge->toNode, visited, command, pool); });
					visited->increment_added();
				}
			}
		}
	}

	if (visited->visited_all())
	{
		if (!visited->test_and_set_end_time_writen()) 
		{
			command->result_report.end_time = chrono::system_clock::now();
			command->sem.post();
		}
	}

}
