#pragma once

#include <vector>

class VisitedArray
{
public:
	VisitedArray(size_t number_of_nodes) 
		:number_of_nodes(number_of_nodes), 
		visited(number_of_nodes),
		number_of_visited(0) {}
	virtual ~VisitedArray() {}

	virtual bool test_and_set_visited(int node_id)
	{
		bool old = visited[node_id];
		visited[node_id] = true;
		return old;
	}

	virtual bool is_visited(int node_id)
	{
		return visited[node_id];
	}

	virtual bool increment_visited()
	{
		number_of_visited++;
		return number_of_visited >= number_of_nodes;
	}

protected:
	const size_t number_of_nodes;
	std::vector<bool> visited;
	size_t number_of_visited;
};
