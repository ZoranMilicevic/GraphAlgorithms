#include <string>
#include <sstream>
#include "DFS.h"
#include "BFS.h"

enum class State 
{
	NONE,
	READING_NODES,
	READING_EDGES
};

void ServerCommand::create_graph_from_string(const std::string& graph_str)
{
	std::istringstream iss(graph_str);
	std::string token;
	State state = State::NONE;
	std::vector<std::shared_ptr<GraphNode>> nodes;

	while (std::getline(iss, token, ' ')) 
	{
		if (state == State::NONE && token == "nodes:")
			state = State::READING_NODES;
		else if (state == State::READING_NODES && token == "edges:")
			state = State::READING_EDGES;
		else if (state == State::READING_EDGES && token == "end")
			break;
		else 
		{
			if (state == State::READING_NODES) 
			{
				nodes.push_back(std::make_shared<GraphNode>(std::stoi(token)));
			}
			else if(state == State::READING_EDGES)
			{
				size_t pos = token.find_first_of(':');
				int key1 = std::stoi(token.substr(0, pos));
				int key2 = std::stoi(token.substr(pos + 1));
				nodes[key1]->addNeighbour(nodes[key2]);
				if(!directed_graph)
					nodes[key2]->addNeighbour(nodes[key1]);
			}
		}
	}

	graph_root = nodes[root_key];
}

