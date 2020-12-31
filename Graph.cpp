#include <thread>
#include <chrono>
#include "Graph.h"
#include "ServerCommand.h"

void GraphNode::traverseNode(ServerCommand& command)
{
	if (command.include_node_reports) 
	{
		std::shared_ptr<NodeResultReport> node_res_rep(new NodeResultReport(key, std::this_thread::get_id(), std::chrono::system_clock::now()));
		command.result_report.add_node_result_report(node_res_rep);
	}

	if (command.node_traverse_time > 0) 
	{
		std::chrono::microseconds node_traverse_time{ command.node_traverse_time };
		std::this_thread::sleep_for(node_traverse_time);
	}
}

Graph::Graph(std::string graph_str, unsigned root_key)
{
	std::istringstream iss(graph_str);
	std::string token;

	while (std::getline(iss, token, ' ') && token != "nodes:");

	//read nodes
	while (std::getline(iss, token, ' ') && token != "edges:")
		all_nodes.push_back(std::make_shared<GraphNode>(std::stoi(token)));

	//read edges
	while(std::getline(iss, token, ' ') && token != "end")
	{
		size_t pos_first = token.find_first_of(':');
		size_t pos_second = token.find_last_of(':');
		int key1 = std::stoi(token.substr(0, pos_first));
		int key2 = 0;
		int weight = 0;
		if (pos_first != pos_second)
		{
			key2 = std::stoi(token.substr(pos_first + 1, pos_second));
			weight = std::stoi(token.substr(pos_second + 1));
		}
		else
		{
			key2 = std::stoi(token.substr(pos_first + 1));
		}
		std::shared_ptr<GraphEdge> edge = std::make_shared<GraphEdge>(all_nodes[key1], all_nodes[key2], weight);
		all_edges.push_back(edge);
		all_nodes[key1]->outgoingEdges.push_back(edge);
		all_nodes[key2]->outgoingEdges.push_back(edge);
	}

	root_node = all_nodes[root_key];
}
