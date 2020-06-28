#pragma once

#include <string>
#include <vector>
#include <map>

enum class GraphAlgorithm
{
	BFS_SINGLE,
	DFS_SINGLE,
	BFS_CPP,
	DFS_CPP

};

inline static const std::map<std::string, GraphAlgorithm>string_to_graph_algorithm =
{
	{"bfsSingle", GraphAlgorithm::BFS_SINGLE},
	{"dfsSingle", GraphAlgorithm::DFS_SINGLE},
	{"bfsCpp", GraphAlgorithm::BFS_CPP},
	{"dfsCpp", GraphAlgorithm::DFS_CPP}
};

inline static const std::vector<std::string> graph_algorithm_to_string =
{
	"BFS SINGLE",
	"DFS_SINGLE",
	"BFS_CPP",
	"DFS_CPP"
};
