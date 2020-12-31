#include "DFS_MT_Command.h"

void DFS_MT_Command::do_command()
{
	std::vector<std::thread> vector_of_worker_threads;
	std::vector<std::shared_ptr<DataStack<GraphNode>>> vector_of_starting_stacks;
	std::shared_ptr<VisitedArrayThreadsafe> visited(new VisitedArrayThreadsafe(graph.all_nodes.size()));

	//create initial node stacks for threads
	for (size_t i = 0; i < number_of_threads; i++)
		vector_of_starting_stacks.push_back(std::make_shared<DataStack<GraphNode>>());
	vector_of_starting_stacks[0]->push(*graph.root_node);

	result_report.start_time = std::chrono::system_clock::now();

	//create threads
	for (size_t i = 0; i < number_of_threads; i++)
		vector_of_worker_threads.push_back(std::thread(&DFS_MT_Command::DFS_MT_traversal, this, vector_of_starting_stacks[i]));

	//join threads
	for (auto&& worker_thr : vector_of_worker_threads)
	{
		if (worker_thr.joinable())
			worker_thr.join();
	}
}

void DFS_MT_Command::DFS_MT_traversal(std::shared_ptr<DataStack<GraphNode>> stack)
{
	unsigned nodes_visited_since_last_split = 0;
	std::shared_ptr<std::mutex> ssr_mutex = std::make_shared<std::mutex>();
	std::shared_ptr<std::condition_variable> ssr_cond_var = std::make_shared<std::condition_variable>();

	while (true)
	{
		if (!stack->empty())
		{
			std::shared_ptr<GraphNode> curNode = stack->pop();

			if (!visited->test_and_set_visited(curNode->key))
			{
				curNode->traverseNode(*this);
				visited->increment_visited();
				nodes_visited_since_last_split++;

				if (!visited->added_all()) {
					for (auto&& edge : curNode->outgoingEdges)
					{
						if (!visited->test_and_set_added(edge->node2->key))
						{
							stack->push(*edge->node2);
							visited->increment_added();
						}
					}
				}
			}

			if (visited->visited_all())
			{
				if (!visited->test_and_set_end_time_writen())
					result_report.end_time = std::chrono::system_clock::now();

				//empty the existing requests to unblock the threads
				std::shared_ptr<StackSplitRequest> req = stack_split_req_queue.try_pop();
				while (req != nullptr)
				{
					req->ssr_cond_var->notify_all();
					req = stack_split_req_queue.try_pop();
				}
				return;
			}

			if (nodes_visited_since_last_split > sufficiency_param && stack->size() > polling_param)
			{
				std::shared_ptr<StackSplitRequest> req = stack_split_req_queue.try_pop();
				if (req != nullptr)
				{
					req->ssr_mutex->lock();
					stack->split(req->to_fill, polling_param);
					req->ssr_mutex->unlock();

					req->ssr_cond_var->notify_all();
					nodes_visited_since_last_split = 0;
				}

			}
		}
		else if (!visited->visited_all())
		{
			stack_split_req_queue.push(StackSplitRequest(stack, ssr_mutex, ssr_cond_var));
			std::unique_lock<std::mutex> ul(*ssr_mutex);
			ssr_cond_var->wait(ul, [&] {return !stack->empty() || visited->visited_all(); });
		}
		else
		{
			break;
		}
	}
}
