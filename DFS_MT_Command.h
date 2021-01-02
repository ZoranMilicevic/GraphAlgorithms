#pragma once

#include <memory>
#include <vector>
#include "ServerCommand.h"
#include "VisitedArrayThreadsafe.h"
#include "DequeThreadsafe.h"
#include "Semaphore.h"

struct StackSplitRequest
{
	StackSplitRequest(std::vector<std::shared_ptr<GraphNode>>* stack_to_fill, FastSemaphore* ssr_sem)
		: stack_to_fill(stack_to_fill), ssr_sem(ssr_sem) {}

	void fulfill_request(std::vector<std::shared_ptr<GraphNode>>& stack_to_fill, unsigned transfer);

	std::vector<std::shared_ptr<GraphNode>>* stack_to_fill;
	FastSemaphore* ssr_sem;
};

class DFS_MT_Command : public ServerCommand
{
public:
	DFS_MT_Command(const std::string& graph_str, unsigned root_key, unsigned node_traverse_time, bool include_node_reports,
		unsigned number_of_threads, unsigned polling_param, unsigned sufficiency_param) 
		: ServerCommand(graph_str, root_key, node_traverse_time, include_node_reports),
		number_of_threads(number_of_threads), polling_param(polling_param), sufficiency_param(sufficiency_param),
		visited(graph.all_nodes.size()) {}

	virtual void do_command() override;

private:
	void DFS_MT_traversal(std::vector<std::shared_ptr<GraphNode>>& starting_stack);

	unsigned number_of_threads;
	unsigned polling_param;
	unsigned sufficiency_param;
	deque_threadsafe<StackSplitRequest> stack_split_req_queue;
	VisitedArrayThreadsafe visited;
};

