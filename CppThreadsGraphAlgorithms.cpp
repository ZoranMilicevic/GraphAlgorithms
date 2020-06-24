#include "CppThreadsGraphAlgorithms.h"
#include "ResultReportCppThreads.h"
using namespace std;


void CppThreadsGraphAlgorithms::DFS(GraphNode* node, int number_of_nodes)
{


}

void CppThreadsGraphAlgorithms::dfs_traversal(GraphNode* node, VisitedArrayCppThreads* visited)
{

}

void CppThreadsGraphAlgorithms::BFS(GraphNode* node, int number_of_nodes)
{
	VisitedArrayCppThreads* visited = new VisitedArrayCppThreads(number_of_nodes);
	ThreadPool::getInstance()->submit([=]()->void { CppThreadsGraphAlgorithms::bfs_traversal(node, visited); });
}

void CppThreadsGraphAlgorithms::bfs_traversal(GraphNode* node, VisitedArrayCppThreads* visited)
{
	if (!visited->test_and_set_visited(node->key))
	{
		node->traverseNode();
		visited->increase_visited();
	}

	if (visited->visited_all())
	{
		//todo
		return;
	}

	for(GraphNode* neighbour : node->neighbours)
	{
		if (!visited->test_and_set_added(neighbour->key))
		{
			ThreadPool::getInstance()->submit([=]()->void { CppThreadsGraphAlgorithms::bfs_traversal(neighbour, visited); });
		}
	}

}


