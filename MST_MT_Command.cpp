#include "MST_MT_Command.h"
#include <algorithm>

void MST_MT_Command::do_command()
{
	result_report.start_time = std::chrono::system_clock::now();

	std::vector<MST_Tree> trees(graph.all_nodes.size());
	size_t number_of_valid_trees = graph.all_nodes.size();

	//initially each tree contains only one node
	for (auto&& node : graph.all_nodes)
		trees[node->key] = MST_Tree(node);

	while (number_of_valid_trees > 1)
	{
		//for each tree find smallest outgoing edge
		//and the tree with which this tree should be merged
		for (auto& tree : trees)
		{
			pool.submit([&]() -> void { 
				tree.find_smallest_outgoing_edge();
				tree.find_tree_to_merge_with(trees);
				sem.post();
			});
		}

		//wait for all smallest edges to be found
		for(int i = 0; i<trees.size(); i++)
			sem.wait();

		//merge trees
		for (auto&& tree : trees)
		{
			auto tree1 = tree.get_tree_for_merging();
			auto tree2 = tree.tree_to_merge_with->get_tree_for_merging();

			if (tree1->tree_id != tree2->tree_id)
			{
				tree1->merge_with_tree(*tree2);
				number_of_valid_trees--;
			}
		}

		//erase trees that were merget into other trees
		trees.erase(
			std::remove_if(trees.begin(), trees.end(), [](const MST_Tree& tree) {return tree.merged_with_tree != nullptr; }),
			trees.end()
		);
	}

	mst = trees[0];

	result_report.end_time = std::chrono::system_clock::now();
}

std::string MST_MT_Command::get_result_string()
{
	return result_report.to_string() + mst.to_string();
}
