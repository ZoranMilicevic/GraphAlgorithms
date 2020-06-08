#pragma once

#include <atomic>
#include <shared_mutex>



class VisitedArrayCppThreads
{
public:
	VisitedArrayCppThreads(int number_of_nodes)
		:sh_mutex_array(new std::shared_mutex[number_of_nodes]()),
		number_of_nodes(number_of_nodes),
		visited(new int[number_of_nodes]()),
		number_of_visited(0){}

	virtual ~VisitedArrayCppThreads() 
	{
		delete visited;
	}

	virtual bool test_and_set_visited(int node_id)
	{
		sh_mutex_array[node_id].lock();
		int retval = visited[node_id];
		visited[node_id] = 2;
		sh_mutex_array[node_id].unlock();

		return retval == 2;
	}

	virtual bool is_visited(int node_id) const
	{
		sh_mutex_array[node_id].lock_shared();
		int retval = visited[node_id];
		sh_mutex_array[node_id].unlock_shared();
		return retval == 2;
	}

	virtual void set_visited(int node_id)
	{
		sh_mutex_array[node_id].lock();
		visited[node_id] = 2;
		sh_mutex_array[node_id].unlock();
	}


	bool test_and_set_added(int node_id)
	{
		sh_mutex_array[node_id].lock();
		int retval = visited[node_id];
		visited[node_id] = 1;
		sh_mutex_array[node_id].unlock();

		return retval == 1 || retval == 2;
	}

	virtual bool is_added(int node_id) const
	{
		sh_mutex_array[node_id].lock_shared();
		int retval = visited[node_id];
		sh_mutex_array[node_id].unlock_shared();
		return retval == 1 || retval == 2;
	}

	virtual void set_added(int node_id)
	{
		sh_mutex_array[node_id].lock();
		visited[node_id] = 1;
		sh_mutex_array[node_id].unlock();
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
	int* visited;
	std::atomic<int> number_of_visited;
};
