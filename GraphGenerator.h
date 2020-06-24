#pragma once

#include <vector>
#include <string>
#include <utility>
#include "ConfigurationParameters.h"

#include "GraphNode.h"

class GraphGenerator
{
public:
	static GraphNode* generate_graph(std::vector<int> node_keys, std::vector<std::pair<int, int>> edges, int root_key = 0);
};
