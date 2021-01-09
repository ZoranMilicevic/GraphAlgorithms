#include "DFS_MT_Command.h"
#include "Graph.h"

void StackSplitRequest::fulfill_request(std::vector<std::shared_ptr<GraphNode>>& stack, unsigned transfer)
{
	for (size_t i = 0; i < transfer; i++) 
	{
		stack_to_fill->push_back(stack.back());
		stack.pop_back();
	}
}

void DFS_MT_Command::do_command()
{
	result_report.start_time = std::chrono::system_clock::now();

	std::vector<std::thread> vector_of_worker_threads;
	std::vector<std::vector<std::shared_ptr<GraphNode>>> vector_of_starting_stacks(number_of_threads);

	vector_of_starting_stacks[0].push_back(graph.root_node);

	//create threads
	for (size_t i = 0; i < number_of_threads; i++)
		vector_of_worker_threads.push_back(std::thread(&DFS_MT_Command::DFS_MT_traversal, this, std::ref(vector_of_starting_stacks[i])));

	//join threads
	for (auto&& worker_thr : vector_of_worker_threads)
	{
		if (worker_thr.joinable())
			worker_thr.join();
	}
}

void DFS_MT_Command::DFS_MT_traversal(std::vector<std::shared_ptr<GraphNode>>& stack)
{
	unsigned nodes_visited_since_last_split = 0;
	FastSemaphore ssr_sem;

	while (true)
	{
		if (!stack.empty())
		{
			std::shared_ptr<GraphNode> curNode = stack.back();
			stack.pop_back();

			if (!visited.test_and_set_visited(curNode->key))
			{
				curNode->traverseNode(node_traverse_time);
				if (include_node_reports)
					result_report.add_node_result_report(NodeResultReport(curNode->key, std::this_thread::get_id(), std::chrono::system_clock::now()));

				visited.increment_visited();
				nodes_visited_since_last_split++;

				if (!visited.added_all()) {
					for (auto&& edge : curNode->outgoingEdges)
					{
						if (!visited.test_and_set_added(edge->node2->key))
						{
							stack.push_back(edge->node2);
							visited.increment_added();
						}
					}
				}
			}

			if (visited.visited_all())
			{
				if (!visited.test_and_set_end_time_writen())
					result_report.end_time = std::chrono::system_clock::now();

				//empty the existing requests to unblock the threads
				std::optional<StackSplitRequest> req = stack_split_req_queue.try_pop();
				while (req.has_value())
				{
					req.value().ssr_sem->post();
					req = stack_split_req_queue.try_pop();
				}
				return;
			}

			if (nodes_visited_since_last_split > sufficiency_param && stack.size() > polling_param)
			{
				std::optional<StackSplitRequest> req = stack_split_req_queue.try_pop();
				if (req.has_value())
				{
					req.value().fulfill_request(stack, polling_param);
					req.value().ssr_sem->post();
					nodes_visited_since_last_split = 0;
				}

			}
		}
		else if (!visited.visited_all())
		{
			stack_split_req_queue.push(StackSplitRequest(&stack, &ssr_sem));
			ssr_sem.wait();
		}
		else
		{
			break;
		}
	}
}

