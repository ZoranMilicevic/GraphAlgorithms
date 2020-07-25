#include <vector>
#include <memory>
#include <chrono>
#include "DataStackSingleThread.h"
#include "GraphNode.h"
#include "VisitedArraySingleThread.h"
#include "DFS.h"

using namespace std;

void DFS::DFS_ST(const std::shared_ptr<ServerCommand>& command)
{
	command->result_report.start_time = chrono::system_clock::now();

	DataStackSingleThread<GraphNode> stack;
	VisitedArraySingleThread visited(command->number_of_nodes);

	stack.push(*command->graph_root);

	while (!stack.empty())
	{
		GraphNode* curNode = stack.pop();

		if (!visited.test_and_set_visited(curNode->key))
		{
			curNode->traverseNode(command);
			if (visited.increment_visited())
				break;
		}

		for (auto&& neighbour : curNode->neighbours) {
			if (!visited.is_visited(neighbour->key))
				stack.push(*neighbour);
		}
	}

	command->result_report.end_time = chrono::system_clock::now();
	command->result_report.elapsed_time = command->result_report.end_time - command->result_report.start_time;
}

void DFS::DFS_MT(const std::shared_ptr<ServerCommand>& command)
{

}

void DFS::DFS_MT_traversal(const std::shared_ptr<GraphNode>& node, std::shared_ptr<VisitedArrayCppThreads> visited, const std::shared_ptr<ServerCommand>& command)
{
	DataStackSingleThread<GraphNode> stack;

	while(true)
	{
		if(!stack.empty())
		{
			
		}
	}

}
