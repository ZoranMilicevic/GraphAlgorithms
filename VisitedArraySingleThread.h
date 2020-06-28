#pragma once

class VisitedArraySingleThread
{
public:
	VisitedArraySingleThread(int number_of_nodes) 
		:number_of_nodes(number_of_nodes), 
		visited(new bool[number_of_nodes]()),
		number_of_visited(0) {}
	virtual ~VisitedArraySingleThread() 
	{
		delete visited;
	}

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
	const int number_of_nodes;
	bool* visited;
	int number_of_visited;

};
