#pragma once

#include <atomic>
#include <shared_mutex>

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
		number_of_nodes(number_of_nodes),
		visited(new NodeState[number_of_nodes]()),
		number_of_visited(0){}

	virtual ~VisitedArrayCppThreads() 
	{
		delete visited;
		//delete sh_mutex_array;
	}

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

	virtual bool visited_all()
	{
		return number_of_visited == number_of_nodes;
	}

protected:
	mutable std::shared_mutex* sh_mutex_array;
	const int number_of_nodes;
	NodeState* visited;
	std::atomic<int> number_of_visited;
};
