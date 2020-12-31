#include "MST_Component.h"

void MST_Component::find_smallest_outgoing_edge()
{
	smallest_outgoing_edge = nullptr;
	node_on_other_side_of_smallest_edge = nullptr;
	for (auto&& node : nodes)
	{
		for (auto&& edge : node->outgoingEdges)
		{
			if (edges.find(edge) == edges.end()) //todo ??
			{
				if (smallest_outgoing_edge == nullptr || smallest_outgoing_edge->weight > edge->weight)
				{
					smallest_outgoing_edge = edge;
					if (node->key == smallest_outgoing_edge->node1->key)
						node_on_other_side_of_smallest_edge = smallest_outgoing_edge->node2;
					else
						node_on_other_side_of_smallest_edge = smallest_outgoing_edge->node1;
				}
			}
		}
	}
}

void MST_Component::find_component_to_merge_with(const std::vector<MST_Component>& components)
{
	for (auto&& component : components)
	{
		if (component.component_id != component_id && component.is_valid()
			&& component.nodes.find(node_on_other_side_of_smallest_edge) != component.nodes.end())
		{
			component_to_merge_with = component.component_id;
		}
	}
}

void MST_Component::merge_with_component(MST_Component& component)
{
	component.merged_into_component = component_id;
	nodes.insert(component.nodes.begin(), component.nodes.end());
	edges.insert(component.edges.begin(), component.edges.end());
	component_to_merge_with = -1;
	number_of_valid_components--;
}
