#pragma once

#include <string>

enum class GraphAlgorithm
{
	BFS_SINGLE,
	DFS_SINGLE,
	BFS_CPP,
	DFS_CPP

};

std::string graph_algorithm_to_string(GraphAlgorithm ga)
{
	switch (ga)
	{
	case GraphAlgorithm::BFS_SINGLE:
		return "BFS SINGLE";
	case GraphAlgorithm::DFS_SINGLE:
		return "DFS_SINGLE";
	case GraphAlgorithm::BFS_CPP:
		return "BFS_CPP";
	case GraphAlgorithm::DFS_CPP:
		return "DFS_CPP";
	default:
		return "";
	}
}