#include "CppThreadsGraphAlgorithms.h"
using namespace std;


void CppThreadsGraphAlgorithms::DFS(ServerCommand* command)
{


}

void CppThreadsGraphAlgorithms::dfs_traversal(GraphNode* node, VisitedArrayCppThreads* visited)
{

}

void CppThreadsGraphAlgorithms::BFS(ServerCommand* command)
{
	VisitedArrayCppThreads* visited = new VisitedArrayCppThreads(command->number_of_nodes);
	ThreadPool::getInstance(command->number_of_threads)->submit([=]()->void { CppThreadsGraphAlgorithms::bfs_traversal(command->graph_root, visited, command->node_traverse_time); });
	ThreadPool::destroy_pool();
}

void CppThreadsGraphAlgorithms::bfs_traversal(GraphNode* node, VisitedArrayCppThreads* visited, int traverse_time)
{
	if (!visited->test_and_set_visited(node->key))
	{
		node->traverseNode(traverse_time);
		visited->increase_visited();
	}

	if (visited->visited_all())
	{
		ResultReport::get_cur_repport()->end_time = chrono::system_clock::now();
		ResultReport::get_cur_repport()->elapsed_time = ResultReport::get_cur_repport()->end_time - ResultReport::get_cur_repport()->start_time;
		ThreadPool::getInstance()->stopThreads();
		return;
	}

	for(GraphNode* neighbour : node->neighbours)
	{
		if (!visited->test_and_set_added(neighbour->key))
		{
			ThreadPool::getInstance()->submit([=]()->void { CppThreadsGraphAlgorithms::bfs_traversal(neighbour, visited, traverse_time); });
		}
	}

}


