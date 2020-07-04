#include <iostream>
#include "GraphGenerator.h"
#include "GraphNode.h"

using namespace std;

GraphNode* GraphGenerator::generate_graph(
	std::vector<int> node_keys, 
	std::vector<std::pair<int, int>> edges, 
	int root_key
)
{
	vector<GraphNode*> nodes;

	for(int key : node_keys)
	{
		nodes.push_back(new GraphNode(key));
	}

	for(auto&& edge: edges)
	{
		int key1 = edge.first;
		int key2 = edge.second;

		nodes[key1]->addNeighbour(nodes[key2]);
		nodes[key2]->addNeighbour(nodes[key1]);
	}

	return nodes[root_key];
}
