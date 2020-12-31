#pragma once

#include <memory>
#include "ServerCommand.h"
#include "VisitedArrayThreadsafe.h"
#include "DataStack.h"
#include "DataQueueThreadsafe.h"

struct StackSplitRequest
{
	StackSplitRequest(
		std::shared_ptr<DataStack<GraphNode>> to_fill,
		std::shared_ptr<std::mutex> ssr_mutex,
		std::shared_ptr<std::condition_variable> ssr_cond_var
	) : to_fill(to_fill), ssr_mutex(ssr_mutex), ssr_cond_var(ssr_cond_var) {}

	std::shared_ptr<DataStack<GraphNode>> to_fill;
	std::shared_ptr<std::mutex> ssr_mutex;
	std::shared_ptr<std::condition_variable> ssr_cond_var;
};

class DFS_MT_Command : public ServerCommand
{
public:
	DFS_MT_Command(const std::string& graph_str, unsigned root_key, unsigned node_traverse_time, bool include_node_reports,
		unsigned number_of_threads, unsigned polling_param, unsigned sufficiency_param) 
		: ServerCommand(graph_str, root_key, node_traverse_time, include_node_reports),
		number_of_threads(number_of_threads), polling_param(polling_param), sufficiency_param(sufficiency_param),
		visited(new VisitedArrayThreadsafe(graph.all_nodes.size())) {}

	virtual void do_command() override;

private:
	void DFS_MT_traversal(std::shared_ptr<DataStack<GraphNode>> starting_stack);

	unsigned number_of_threads;
	unsigned polling_param;
	unsigned sufficiency_param;
	DataQueueThreadsafe<StackSplitRequest> stack_split_req_queue;
	std::shared_ptr<VisitedArrayThreadsafe> visited;
};

