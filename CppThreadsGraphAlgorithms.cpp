#include "CppThreadsGraphAlgorithms.h"
using namespace std;

VisitedArrayCppThreads* CppThreadsGraphAlgorithms::visited = NULL;

void CppThreadsGraphAlgorithms::DFS()
{


}

void CppThreadsGraphAlgorithms::BFS()
{
	ThreadPool::getInstance()->submit([=]()->void { CppThreadsGraphAlgorithms::bfs_traversal(root); });
}

void CppThreadsGraphAlgorithms::bfs_traversal(GraphNode* node)
{
	if (!visited->test_and_set_visited(node->key))
	{
		node->traverseNode();
		visited->increase_visited();
		if (visited->visited_all())
		{
			ThreadPool::getInstance()->stopThreads();
		}
	}
	for(GraphNode* neighbour : node->neighbours)
	{
		if (!visited->test_and_set_added(neighbour->key))
		{
			ThreadPool::getInstance()->submit([=]()->void { CppThreadsGraphAlgorithms::bfs_traversal(neighbour); });
		}
	}

}


