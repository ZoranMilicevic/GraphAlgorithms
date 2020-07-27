#pragma once

#include <mutex>
#include <atomic>
#include <memory>
#include <condition_variable>
#include "StackWithSplit.h"
#include "DataQueueCppThreads.h"

class StackSplitRequest
{
public:
	StackSplitRequest(std::shared_ptr<StackWithSplit<GraphNode>> to_fill, std::shared_ptr<std::mutex> ssr_mutex, std::shared_ptr<std::condition_variable> ssr_cond_var) :
		to_fill(to_fill), ssr_mutex(ssr_mutex), ssr_cond_var(ssr_cond_var) {}

	std::shared_ptr<StackWithSplit<GraphNode>> to_fill;
	std::shared_ptr<std::mutex> ssr_mutex;
	std::shared_ptr<std::condition_variable> ssr_cond_var;
};

class DFS_MT_Util
{
public:
	DFS_MT_Util() {};

	DataQueueCppThreads<StackSplitRequest> stack_split_req_queue;
};