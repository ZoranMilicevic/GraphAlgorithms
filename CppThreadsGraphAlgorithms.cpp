#include "CppThreadsGraphAlgorithms.h"
#include "ThreadPool.h"
#include "DataStackSingleThread.h"
using namespace std;


void CppThreadsGraphAlgorithms::DFS(const std::shared_ptr<ServerCommand>& command)
{

}

void CppThreadsGraphAlgorithms::dfs_traversal(const shared_ptr<GraphNode>& node, std::shared_ptr<VisitedArrayCppThreads> visited, const std::shared_ptr<ServerCommand>& command)
{
	DataStackSingleThread<GraphNode> stack;

	while (true)
	{
		if (!stack.empty()) {
			GraphNode* curNode = stack.pop();

			if (!visited->test_and_set_visited(curNode->key))
			{
				curNode->traverseNode(command);
				visited->increase_visited();
			}

			if (visited->visited_all())
			{
				command->result_report.end_time = chrono::system_clock::now();
				command->result_report.elapsed_time = command->result_report.end_time - command->result_report.start_time;
				break;
			}

			for (auto&& neighbour : curNode->neighbours) {
				if (!visited->is_visited(neighbour->key))
					stack.push(*neighbour);
			}
		}
		else 
		{
			
		}
	}

}

void CppThreadsGraphAlgorithms::BFS(const std::shared_ptr<ServerCommand>& command)
{
	shared_ptr<VisitedArrayCppThreads> visited(new VisitedArrayCppThreads(command->number_of_nodes));

	command->result_report.start_time = chrono::system_clock::now();
	ThreadPool::getInstance(command->number_of_threads)->submit(
		[=]()->void { CppThreadsGraphAlgorithms::bfs_traversal(command->graph_root, visited, command); }
	);
	
	ThreadPool::getInstance()->joinThreadsFromPool();
	ThreadPool::destroy_pool();
}

void CppThreadsGraphAlgorithms::bfs_traversal(const shared_ptr<GraphNode>& node, std::shared_ptr<VisitedArrayCppThreads> visited, const std::shared_ptr<ServerCommand>& command)
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

	for(std::shared_ptr<GraphNode> neighbour : node->neighbours)
	{
		if (!visited->test_and_set_added(neighbour->key))
		{
			ThreadPool::getInstance()->submit([=]()->void { CppThreadsGraphAlgorithms::bfs_traversal(neighbour, visited, command); });
		}
	}

}


