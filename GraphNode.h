#pragma once
#include <vector>
#include <thread>
#include <chrono>
#include <memory>
#include "NodeResultReport.h"

class ServerCommand;

class GraphNode : public std::enable_shared_from_this<GraphNode>
{
public:
	struct GraphEdge
	{
		std::shared_ptr<GraphNode> fromNode;
		std::shared_ptr<GraphNode> toNode;
		double weight;

		GraphEdge(const std::shared_ptr<GraphNode>& fromNode, const std::shared_ptr<GraphNode>& toNode, double weight)
			: fromNode(fromNode), toNode(toNode), weight(weight) {}
	};


	int key;
	std::vector<std::shared_ptr<GraphEdge>> outgoingEdges;
	inline static std::vector<std::shared_ptr<GraphEdge>> AllEdges;

	GraphNode(int key) : key(key) {}
	~GraphNode(){}


	void addEdge(std::shared_ptr<GraphNode> to, double weight = 0.0)
	{
		std::shared_ptr<GraphEdge> edge(new GraphEdge(shared_from_this(), to, weight));
		AllEdges.push_back(edge);
		outgoingEdges.push_back(edge);
	}

	void traverseNode(const std::shared_ptr<ServerCommand>& command);
};
