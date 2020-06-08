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
	static GraphGenerator& getInstance();

	GraphNode* generateGraph(const GraphType type, int nodes);

	GraphNode* createCppGraphFromPyGraph(CPyObject py_graph);
	GraphNode* createCppGraphFromNodesAndEdges(const std::vector<int>& nodes, const std::vector<std::pair<int, int>>&);
	
	virtual ~GraphGenerator() {}
private:
	GraphGenerator();

	CPyInstance hInstance;
	CPyObject pModule;
};
