#include "CppThreadsGraphAlgorithms.h"
#include "ThreadPool.h"
using namespace std;


void CppThreadsGraphAlgorithms::DFS(const std::shared_ptr<ServerCommand>&)
{


}

void CppThreadsGraphAlgorithms::dfs_traversal(const shared_ptr<GraphNode>& node, VisitedArrayCppThreads* visited)
{

}

void CppThreadsGraphAlgorithms::BFS(const std::shared_ptr<ServerCommand>& command)
{
	VisitedArrayCppThreads* visited = new VisitedArrayCppThreads(command->number_of_nodes);
	
	command->result_report.start_time = chrono::system_clock::now();
	ThreadPool::getInstance(command->number_of_threads)->submit([=]()->void { CppThreadsGraphAlgorithms::bfs_traversal(command->graph_root, visited, command); });
	
	ThreadPool::getInstance()->joinThreadsFromPool();
	ThreadPool::destroy_pool();
	delete visited;
}

void CppThreadsGraphAlgorithms::bfs_traversal(const shared_ptr<GraphNode>& node, VisitedArrayCppThreads* visited, const std::shared_ptr<ServerCommand>& command)
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


