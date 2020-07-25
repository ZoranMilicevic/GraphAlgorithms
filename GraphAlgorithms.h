#pragma once

#include <string>
#include <vector>
#include <map>

enum class GraphAlgorithm
{
	BFS_ST,
	DFS_ST,
	BFS_MT,
	DFS_MT

};

inline static const std::map<std::string, GraphAlgorithm> string_to_graph_algorithm =
{
	{"BFS_ST", GraphAlgorithm::BFS_ST},
	{"DFS_ST", GraphAlgorithm::DFS_ST},
	{"BFS_MT", GraphAlgorithm::BFS_MT},
	{"DFS_MT", GraphAlgorithm::DFS_MT}
};

inline static const std::vector<std::string> graph_algorithm_to_string =
{
	"BFS_ST",
	"DFS_ST",
	"BFS_MT",
	"DFS_MT"
};
