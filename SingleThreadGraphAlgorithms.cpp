#include "SingleThreadGraphAlgorithms.h"

#include <vector>
#include "DataQueueSingleThread.h"
#include "DataStackSingleThread.h"


using namespace std;

void SingleThreadGraphAlgorithms::DFS()
{
	DataStackSingleThread<GraphNode> stack;

	stack.push(*root);

	while (!stack.empty()) 
	{
		GraphNode* curNode = stack.pop();

		if (!visited->test_and_set_visited(curNode->key))
		{
			curNode->traverseNode();
			if (visited->increment_visited())
				return;
		}

		for (auto&& neighbour : curNode->neighbours) {
			if (!visited->is_visited(neighbour->key))
				stack.push(*neighbour);
		}
	}
}

void SingleThreadGraphAlgorithms::BFS()
{
	DataQueueSingleThread<GraphNode> queue;

	queue.push(*root);

	while (!queue.empty())
	{
		GraphNode* curNode = queue.pop();

		if (!visited->test_and_set_visited(curNode->key))
		{
			curNode->traverseNode();
			if (visited->increment_visited())
				return;
		}

		for (auto&& neighbour : curNode->neighbours) {
			if (!visited->is_visited(neighbour->key))
				queue.push(*neighbour);
		}
	}
}