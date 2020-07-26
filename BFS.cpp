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

		for (auto&& neighbour : curNode->neighbours) {
			if (!visited.is_visited(neighbour->key))
				queue.push(*neighbour);
		}
	}

	command->result_report.end_time = chrono::system_clock::now();
	command->result_report.elapsed_time = command->result_report.end_time - command->result_report.start_time;
}



void BFS::BFS_MT(const shared_ptr<ServerCommand>& command)
{
	shared_ptr<VisitedArrayCppThreads> visited(new VisitedArrayCppThreads(command->number_of_nodes));

	command->result_report.start_time = chrono::system_clock::now();
	ThreadPool::getInstance(command->number_of_threads)->submit(
		[=]()->void { BFS::BFS_MT_traversal(command->graph_root, visited, command); }
	);

	ThreadPool::getInstance()->joinThreadsFromPool();
	ThreadPool::destroy_pool();
}

void BFS::BFS_MT_traversal(const shared_ptr<GraphNode>& node, shared_ptr<VisitedArrayCppThreads> visited, const shared_ptr<ServerCommand>& command)
{
	if (!visited->test_and_set_visited(node->key))
	{
		node->traverseNode(command);
		visited->increase_visited();
	}

	if (visited->visited_all())
	{
		command->result_report.end_time = chrono::system_clock::now();
		command->result_report.elapsed_time = command->result_report.end_time - command->result_report.start_time;
		ThreadPool::getInstance()->stopThreads();
		return;
	}

	for (shared_ptr<GraphNode> neighbour : node->neighbours)
	{
		if (!visited->test_and_set_added(neighbour->key))
		{
			ThreadPool::getInstance()->submit([=]()->void { BFS::BFS_MT_traversal(neighbour, visited, command); });
		}
	}

}
