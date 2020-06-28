#include <vector>
#include <chrono>
#include "SingleThreadGraphAlgorithms.h"
#include "DataQueueSingleThread.h"
#include "DataStackSingleThread.h"


using namespace std;


void SingleThreadGraphAlgorithms::DFS(ServerCommand* command)
{
	ResultReport::get_cur_repport()->start_time = chrono::system_clock::now();
	
	DataStackSingleThread<GraphNode> stack;
	VisitedArraySingleThread visited(command->number_of_nodes);

	stack.push(*command->graph_root);

	while (!stack.empty())
	{
		GraphNode* curNode = stack.pop();

		if (!visited.test_and_set_visited(curNode->key))
		{
			curNode->traverseNode(command->node_traverse_time);
			if (visited.increment_visited())
				break;
		}

		for (auto&& neighbour : curNode->neighbours) {
			if (!visited.is_visited(neighbour->key))
				stack.push(*neighbour);
		}
	}

	ResultReport::get_cur_repport()->end_time = chrono::system_clock::now();
	ResultReport::get_cur_repport()->elapsed_time = ResultReport::get_cur_repport()->end_time - ResultReport::get_cur_repport()->start_time;
}

void SingleThreadGraphAlgorithms::BFS(ServerCommand* command)
{
	ResultReport::get_cur_repport()->start_time = chrono::system_clock::now();

	DataQueueSingleThread<GraphNode> queue;
	VisitedArraySingleThread visited(command->number_of_nodes);

	queue.push(*command->graph_root);

	while (!queue.empty())
	{
		GraphNode* curNode = queue.pop();

		if (!visited.test_and_set_visited(curNode->key))
		{
			curNode->traverseNode(command->node_traverse_time);
			if (visited.increment_visited())
				break;
		}

		for (auto&& neighbour : curNode->neighbours) {
			if (!visited.is_visited(neighbour->key))
				queue.push(*neighbour);
		}
	}

	ResultReport::get_cur_repport()->end_time = chrono::system_clock::now();
	ResultReport::get_cur_repport()->elapsed_time = ResultReport::get_cur_repport()->end_time - ResultReport::get_cur_repport()->start_time;
}
