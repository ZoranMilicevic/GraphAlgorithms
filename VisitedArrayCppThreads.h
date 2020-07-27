#pragma once

#include <atomic>
#include <shared_mutex>
#include <memory>

enum NodeState
{
	Initial,
	AddedToWorkQueue,
	Visited
};

class VisitedArrayCppThreads
{
public:
	VisitedArrayCppThreads(int number_of_nodes)
		:sh_mutex_array(new std::shared_mutex[number_of_nodes]()),
		visited(new NodeState[number_of_nodes]()),
		number_of_nodes(number_of_nodes),
		number_of_visited(0),
		number_of_added(0)
	{
		end_time_written.clear();
	}

	virtual ~VisitedArrayCppThreads() {}

	virtual bool test_and_set_visited(int node_id)
	{
		sh_mutex_array[node_id].lock();
		int retval = visited[node_id];
		visited[node_id] = NodeState::Visited;
		sh_mutex_array[node_id].unlock();

		return retval == NodeState::Visited;
	}

	virtual bool is_visited(int node_id) const
	{
		sh_mutex_array[node_id].lock_shared();
		int retval = visited[node_id];
		sh_mutex_array[node_id].unlock_shared();
		return retval == NodeState::Visited;
	}

	bool test_and_set_added(int node_id)
	{
		sh_mutex_array[node_id].lock();
		int retval = visited[node_id];
		visited[node_id] = NodeState::AddedToWorkQueue;
		sh_mutex_array[node_id].unlock();

		return retval == NodeState::AddedToWorkQueue || retval == NodeState::Visited;
	}

	virtual bool is_added(int node_id) const
	{
		sh_mutex_array[node_id].lock_shared();
		int retval = visited[node_id];
		sh_mutex_array[node_id].unlock_shared();
		return retval == NodeState::AddedToWorkQueue || retval == NodeState::Visited;
	}


	virtual void increase_visited()
	{
		number_of_visited++;
	}

	virtual void increase_added()
	{
		number_of_added;
	}

	virtual bool visited_all()
	{
		return number_of_visited == number_of_nodes;
	}

	virtual bool added_all()
	{
		return number_of_added == number_of_nodes;
	}

	virtual bool test_and_set_end_time_writen()
	{
		return end_time_written.test_and_set();
	}

protected:
	mutable std::shared_ptr<std::shared_mutex[]> sh_mutex_array;
	std::shared_ptr<NodeState[]> visited;
	const int number_of_nodes;
	std::atomic<int> number_of_visited;
	std::atomic<int> number_of_added;
	std::atomic_flag end_time_written;
};
