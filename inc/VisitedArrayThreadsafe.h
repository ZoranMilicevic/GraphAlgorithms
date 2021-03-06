#pragma once

#include <atomic>
#include <memory>

class VisitedArrayThreadsafe
{
public:
	VisitedArrayThreadsafe(size_t number_of_nodes)
		:visited(number_of_nodes),
		added(number_of_nodes),
		number_of_nodes(number_of_nodes),
		number_of_visited(0),
		number_of_added(0)
	{
		for (int i = 0; i < number_of_nodes; i++) 
		{
			visited[i].clear();
			added[i].clear();
		}
		end_time_written.clear();
	}

	virtual ~VisitedArrayThreadsafe() {}

	virtual bool test_and_set_visited(int node_id)
	{
		return visited[node_id].test_and_set();
	}

	bool test_and_set_added(int node_id)
	{
		return added[node_id].test_and_set();
	}

	virtual void increment_visited()
	{
		number_of_visited++;
	}

	virtual void increment_added()
	{
		number_of_added++;
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
	std::vector<std::atomic_flag> visited;
	std::vector<std::atomic_flag> added;
	const size_t number_of_nodes;
	std::atomic_int number_of_visited;
	std::atomic_int number_of_added;
	std::atomic_flag end_time_written;
};
