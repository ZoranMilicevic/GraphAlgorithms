#pragma once
#include <vector>
#include <memory>
#include <string>

class ServerCommand;
class GraphNode;

class GraphEdge
{
public:
	GraphEdge(const std::shared_ptr<GraphNode>& node1, const std::shared_ptr<GraphNode>& node2, int weight)
		: node1(node1), node2(node2), weight(weight) {}

	std::shared_ptr<GraphNode> node1;
	std::shared_ptr<GraphNode> node2;
	int weight;
};

class GraphNode
{
public:
	GraphNode(int key) : key(key) {}

	void traverseNode(ServerCommand& command);

	int key;
	std::vector<std::shared_ptr<GraphEdge>> outgoingEdges;
};

class Graph
{
public:
	Graph(std::string graph_str, unsigned root_key);

	std::shared_ptr<GraphNode> root_node;
	std::vector<std::shared_ptr<GraphNode>> all_nodes;
	std::vector<std::shared_ptr<GraphEdge>> all_edges;
};
