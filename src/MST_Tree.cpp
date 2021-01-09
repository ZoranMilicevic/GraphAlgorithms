#include "MST_Tree.h"
#include <sstream>

bool MST_Tree::chech_if_node_belongs_to_tree(std::shared_ptr<GraphNode> node_to_check) const
{
	for (auto& node : nodes) 
	{
		if (node->key == node_to_check->key)
			return true;
	}
	return false;
}

void MST_Tree::find_smallest_outgoing_edge()
{
	smallest_outgoing_edge = nullptr;
	node_on_other_side_of_smallest_edge = nullptr;
	for (auto&& node : nodes)
	{
		for (auto&& edge : node->outgoingEdges)
		{
			auto node_on_the_other_side_of_edge = node->key == edge->node1->key ? edge->node2 : edge->node1;

			if (!chech_if_node_belongs_to_tree(node_on_the_other_side_of_edge)) //consider only edges not belonging to the tree
			{
				if (smallest_outgoing_edge == nullptr || smallest_outgoing_edge->weight > edge->weight)
				{
					smallest_outgoing_edge = edge;
					node_on_other_side_of_smallest_edge = node_on_the_other_side_of_edge;
				}
			}
		}
	}
}

void MST_Tree::find_tree_to_merge_with(const std::vector<MST_Tree>& trees)
{
	for (auto& tree : trees)
	{
		if (tree.tree_id != tree_id && tree.chech_if_node_belongs_to_tree(node_on_other_side_of_smallest_edge))
			tree_to_merge_with =(MST_Tree*) &tree;
	}
}

void MST_Tree::merge_with_tree(MST_Tree& tree)
{
	//it is O(1)
	nodes.splice(nodes.end(), tree.nodes);
	edges.splice(edges.end(), tree.edges);
	edges.push_back(smallest_outgoing_edge);
	tree.merged_with_tree = this;
}

MST_Tree* MST_Tree::get_tree_for_merging()
{
	if (merged_with_tree == nullptr)
		return (MST_Tree*)this;
	else
		return merged_with_tree->get_tree_for_merging();
}

std::string MST_Tree::to_string()
{
	std::ostringstream retVal;
	unsigned total_weight = 0;

	retVal << "MST edges are: " << std::endl;
	for (auto&& edge : edges)
	{
		retVal << edge->node1->key << "->" << edge->node2->key << "(" << edge->weight << ")" << std::endl;
		total_weight += edge->weight;
	}

	return "MST total weight = " + std::to_string(total_weight) + "\n" + retVal.str();
}