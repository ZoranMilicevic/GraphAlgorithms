#pragma once

#include <utility>
#include <string>
#include "ConfigurationParameters.h"
#include "CPyObject.h"
#include "CPyInstance.h"
#include "GraphNode.h"
#include "GraphType.h"

class GraphGenerator
{
public:
	static GraphGenerator& get_instance();

	GraphNode* generate_graph(const GraphType type, int nodes);

	GraphNode* create_cpp_graph_from_py_graph(CPyObject py_graph);
	GraphNode* create_cpp_graph_from_nodes_and_edges(const std::vector<int>& nodes, const std::vector<std::pair<int, int>>&);
	
	virtual ~GraphGenerator() {}
private:
	GraphGenerator();

	CPyInstance hInstance;
	CPyObject pModule;
};
