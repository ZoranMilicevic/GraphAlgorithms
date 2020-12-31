#pragma once

#include "Graph.h"
#include <set>
#include <algorithm>
#include <vector>
#include <string>

class MST_Component {
public:
	inline static unsigned number_of_valid_components;

	MST_Component(std::shared_ptr<GraphNode> node) : component_id(node->key), merged_into_component(-1)
	{
		nodes.insert(node);
	}

	void find_smallest_outgoing_edge();
	void find_component_to_merge_with(const std::vector<MST_Component>& components);
	void merge_with_component(MST_Component& component);
	bool is_valid() const { return merged_into_component != -1; }
	unsigned get_component_to_merge_with() const { return component_to_merge_with; }

private:
	unsigned component_id;
	std::set<std::shared_ptr<GraphNode>> nodes;
	std::set<std::shared_ptr<GraphEdge>> edges;
	std::shared_ptr<GraphEdge> smallest_outgoing_edge;
	std::shared_ptr<GraphNode> node_on_other_side_of_smallest_edge;
	unsigned component_to_merge_with;
	int merged_into_component;
};


