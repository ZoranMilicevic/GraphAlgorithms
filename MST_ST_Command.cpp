#include "MST_ST_Command.h"
#include "MST_Component.h"

//int find_tree_root_node_belongs_to(std::vector<std::pair<int, int>>& trees, int node_key)
//{
//	if (trees[node_key].second != node_key)
//		trees[node_key].second = find_tree_root_node_belongs_to(trees, trees[node_key].second);
//	return trees[node_key].second;
//}
//
//void union_of_trees(std::vector<std::pair<int, int>>& trees, int treeA, int treeB)
//{
//	int rootA = find_tree_root_node_belongs_to(trees, treeA);
//	int rootB = find_tree_root_node_belongs_to(trees, treeB);
//	// merge smaller tree to larger one by comparing rank
//	if (trees[rootA].first < trees[rootB].first)
//		trees[rootA].second = rootB;
//	else if (trees[rootA].first > trees[rootB].first)
//		trees[rootB].second = rootA;
//	else // If ranks are same
//	{
//		trees[rootB].second = rootA;
//		trees[rootA].first++;
//	}
//}
//
//void MST_ST_Command::do_command()
//{
//	size_t number_of_trees = graph.all_nodes.size();
//	//pair represents rank : root_node_id
//	std::vector<std::pair<int, int>> trees;
//
//	//create single node trees
//	for (int i = 0; i < number_of_trees; i++)
//		trees.push_back(std::make_pair(0, i));
//
//	while (number_of_trees > 1)
//	{
//		std::vector<std::shared_ptr<GraphEdge>> smallest_edge_for_each_tree(graph.all_nodes.size(), nullptr);
//		for (auto&& edge : graph.all_edges)
//		{
//			//find the trees to which nodes connected by edge belong to
//			int rootA = find_tree_root_node_belongs_to(trees, edge->node1->key);
//			int rootB = find_tree_root_node_belongs_to(trees, edge->node2->key);
//
//			if (rootA != rootB) //belong to different trees
//			{
//				if (smallest_edge_for_each_tree[rootA] == nullptr || smallest_edge_for_each_tree[rootA]->weight > edge->weight)
//					smallest_edge_for_each_tree[rootA] = edge;
//				if (smallest_edge_for_each_tree[rootB] == nullptr || smallest_edge_for_each_tree[rootB]->weight > edge->weight)
//					smallest_edge_for_each_tree[rootB] = edge;
//			}
//		}
//
//		for (auto&& smallest_edge : smallest_edge_for_each_tree)
//		{
//			if (smallest_edge != nullptr)
//			{
//				int rootA = find_tree_root_node_belongs_to(trees, smallest_edge->node1->key);
//				int rootB = find_tree_root_node_belongs_to(trees, smallest_edge->node2->key);
//
//				if (rootA != rootB)
//				{
//					union_of_trees(trees, rootA, rootB);
//					mst_total_weight += smallest_edge->weight;
//					number_of_trees--;
//					mst.push_back(smallest_edge);
//				}
//			}
//		}
//	}
//}



void MST_ST_Command::do_command() 
{
	std::vector<MST_Component> components(graph.all_nodes.size());
	MST_Component::number_of_valid_components = graph.all_nodes.size();

	for (auto&& node : graph.all_nodes) 
	{
		components[node->key] = MST_Component(node);
	}
	
	while (MST_Component::number_of_valid_components > 1) 
	{
		//for each component find smallest outgoing edge
		//and the component with which it should merge
		for (size_t i = 0; i < components.size(); i++)
		{
			if (components[i].is_valid())
			{
				components[i].find_smallest_outgoing_edge();
				components[i].find_component_to_merge_with(components);
			}
		}

		//merge components
		for (size_t i = 0; i < components.size(); i++)
		{
			if (components[i].is_valid())
			{
				MST_Component& component_to_merge_with = components[components[i].get_component_to_merge_with()];
				components[i].merge_with_component(component_to_merge_with);
			}
		}
	}

}

std::string MST_ST_Command::get_result_string()
{
	std::ostringstream res_string;
	res_string << std::endl << "MST total weight = " << mst_total_weight << std::endl;

	res_string << "MST edges are: " << std::endl;
	for (auto&& edge : mst)
		res_string << edge->node1->key << ":" << edge->node2->key << ":" << edge->weight << std::endl;

	return result_report.to_string() + res_string.str();
}
