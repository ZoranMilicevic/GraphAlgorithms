#pragma once

#include "Graph.h"
#include <set>
#include <algorithm>
#include <vector>
#include <string>
#include <list>

class MST_Tree 
{
public:	
	MST_Tree() : tree_id(0), smallest_outgoing_edge(nullptr),
		node_on_other_side_of_smallest_edge(nullptr), tree_to_merge_with(nullptr), merged_with_tree(nullptr) {}
	
	MST_Tree(std::shared_ptr<GraphNode> node) : tree_id(node->key), smallest_outgoing_edge(nullptr),
		node_on_other_side_of_smallest_edge(nullptr), tree_to_merge_with(nullptr), merged_with_tree(nullptr)
	{
		nodes.push_back(node);
	}

	bool chech_if_node_belongs_to_tree(std::shared_ptr<GraphNode> node_to_check) const;
	void find_smallest_outgoing_edge();
	void find_tree_to_merge_with(const std::vector<MST_Tree>& trees);
	void merge_with_tree(MST_Tree& tree);
	MST_Tree* get_tree_for_merging();
	std::string to_string();


	unsigned tree_id;
	std::list<std::shared_ptr<GraphNode>> nodes;
	std::list<std::shared_ptr<GraphEdge>> edges;
	std::shared_ptr<GraphEdge> smallest_outgoing_edge;
	std::shared_ptr<GraphNode> node_on_other_side_of_smallest_edge;

	MST_Tree* tree_to_merge_with;
	MST_Tree* merged_with_tree;
};


