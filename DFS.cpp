#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
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
		std::shared_ptr<GraphNode> curNode = stack.pop();

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
	vector<thread> vector_of_worker_threads;
	vector<shared_ptr<StackWithSplit<GraphNode>>> vector_of_starting_stacks;
	shared_ptr<VisitedArrayCppThreads> visited(new VisitedArrayCppThreads(command->number_of_nodes));
	shared_ptr<DFS_MT_Util> util_struct = make_shared<DFS_MT_Util>();

	//create initial node stacks for threads
	for(int i = 0; i < command->number_of_threads; i++)
		vector_of_starting_stacks.push_back(make_shared<StackWithSplit<GraphNode>>());
	vector_of_starting_stacks[0]->push(*command->graph_root);

	command->result_report.start_time = chrono::system_clock::now();

	//create threads
	for(int i = 0; i < command->number_of_threads; i++)
		vector_of_worker_threads.push_back(
			thread(DFS::DFS_MT_traversal, vector_of_starting_stacks[i], visited, command, util_struct));

	//join threads
	for(auto&& worker_thr:vector_of_worker_threads)
	{
		if (worker_thr.joinable())
			worker_thr.join();
	}
	
}

void DFS::DFS_MT_traversal(shared_ptr<StackWithSplit<GraphNode>> stack, shared_ptr<VisitedArrayCppThreads> visited, const shared_ptr<ServerCommand>& command, const shared_ptr<DFS_MT_Util>& util_struct)
{
	int nodes_visited_since_last_split = 0;
	shared_ptr<mutex> ssr_mutex = make_shared<mutex>();
	shared_ptr<condition_variable> ssr_cond_var = make_shared<condition_variable>();

	while(true)
	{
		if(!stack->empty())
		{
			std::shared_ptr<GraphNode> curNode = stack->pop();

			if (!visited->test_and_set_visited(curNode->key))
			{
				curNode->traverseNode(command);
				visited->increment_visited();
				nodes_visited_since_last_split++;

				if (!visited->added_all()) {
					for (auto&& neighbour : curNode->neighbours)
					{
						if (!visited->test_and_set_added(neighbour->key))
						{
							stack->push(*neighbour);
							visited->increment_added();
						}
					}
				}
			}

			if(visited->visited_all())
			{
				if(!visited->test_and_set_end_time_writen())
					command->result_report.end_time = chrono::system_clock::now();
				
				std::shared_ptr<StackSplitRequest> req = util_struct->stack_split_req_queue.try_pop();
				while(req!=nullptr)
				{
					req->ssr_cond_var->notify_all();
					req = util_struct->stack_split_req_queue.try_pop();
				}
				return;
			}

			if(nodes_visited_since_last_split > command->sufficiency_param &&  stack->get_size() > command->polling_param)
			{
				std::shared_ptr<StackSplitRequest> req = util_struct->stack_split_req_queue.try_pop();
				if (req != nullptr) 
				{
					req->ssr_mutex->lock();
					stack->split(req->to_fill, command->polling_param);
					req->ssr_mutex->unlock();

					req->ssr_cond_var->notify_all();
					nodes_visited_since_last_split = 0;
				}
				
			}
		}
		else if(!visited->visited_all())
		{
			util_struct->stack_split_req_queue.push(StackSplitRequest(stack, ssr_mutex, ssr_cond_var));
			std::unique_lock<std::mutex> ul(*ssr_mutex);
			ssr_cond_var->wait(ul, [&] {return !stack->empty() || visited->visited_all(); });
		}
		else 
		{
			break;
		}
	}

}
