#pragma once

#include "GraphNode.h"
#include "ThreadPool.h"
#include "VisitedArrayCppThreads.h"


class CppThreadsGraphAlgorithms{
public:
	CppThreadsGraphAlgorithms(GraphNode* root, int number_of_nodes) 
		:root(root), number_of_nodes(number_of_nodes) {
		visited = new VisitedArrayCppThreads(number_of_nodes);
	};
	virtual ~CppThreadsGraphAlgorithms()
	{
		delete visited;
	}


	virtual void DFS();
	virtual void BFS();

	static void bfs_traversal(GraphNode* node);
	static VisitedArrayCppThreads* visited;

protected:	
	GraphNode* root;
	const int number_of_nodes;
};



