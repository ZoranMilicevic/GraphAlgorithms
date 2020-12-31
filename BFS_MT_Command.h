#pragma once

#include <memory>
#include "ThreadPool.h"
#include "ServerCommand.h"
#include "VisitedArrayThreadsafe.h"

class BFS_MT_Command : public ServerCommand 
{
public:
	BFS_MT_Command(const std::string& graph_str, unsigned root_key, unsigned node_traverse_time, bool include_node_reports,
		unsigned number_of_threads) 
		: ServerCommand(graph_str, root_key, node_traverse_time, include_node_reports), number_of_threads(number_of_threads),
		visited(new VisitedArrayThreadsafe(graph.all_nodes.size())), pool(new ThreadPool(number_of_threads)) {}

	virtual void do_command() override;

private:
	void BFS_MT_traversal(std::shared_ptr<GraphNode> node);
	
	unsigned number_of_threads;
	FastSemaphore sem;
	std::shared_ptr<VisitedArrayThreadsafe> visited;
	std::shared_ptr<ThreadPool> pool;
};

