#include <iostream>
#include <memory>
#include "rpc/server.h"
#include "rpc/this_server.h"
#include "rpc/this_session.h"
#include "ServerCommand.h"
#include "BFS.h"
#include "DFS.h"

std::string bfs_st(unsigned number_of_nodes, unsigned node_traverse_time, unsigned root_key, 
	bool include_node_reports, const std::string& graph_str
)
{
	std::cout << "bfs_st called" << std::endl;

	std::shared_ptr<ServerCommand> command = std::make_shared<ServerCommand>(
		1, number_of_nodes, node_traverse_time, root_key, 0, 0, include_node_reports, graph_str);

	BFS::BFS_ST(command);

	std::cout << "bfs_st done" << std::endl;

	return command->result_report.to_string();
}

std::string dfs_st(unsigned number_of_nodes, unsigned node_traverse_time, unsigned root_key,
	bool include_node_reports, const std::string& graph_str
)
{
	std::cout << "dfs_st called" << std::endl;

	std::shared_ptr<ServerCommand> command = std::make_shared<ServerCommand>(
		1, number_of_nodes, node_traverse_time, root_key, 0, 0, include_node_reports, graph_str);

	DFS::DFS_ST(command);

	std::cout << "dfs_st done" << std::endl;

	return command->result_report.to_string();
}

std::string bfs_mt(
	unsigned number_of_nodes, unsigned node_traverse_time, unsigned root_key, bool include_node_reports, 
	const std::string& graph_str, unsigned number_of_threads
)
{
	std::cout << "bfs_mt called" << std::endl;

	std::shared_ptr<ServerCommand> command = std::make_shared<ServerCommand>(
		number_of_threads, number_of_nodes, node_traverse_time, root_key, 0, 0, include_node_reports, graph_str);

	BFS::BFS_MT(command);

	std::cout << "bfs_mt done" << std::endl;

	return command->result_report.to_string();
}

std::string dfs_mt(
	unsigned number_of_nodes, unsigned node_traverse_time, unsigned root_key, bool include_node_reports, 
	const std::string& graph_str, unsigned number_of_threads, unsigned polling_param, unsigned sufficiency_param
)
{
	std::cout << "dfs_mt called" << std::endl;

	std::shared_ptr<ServerCommand> command = std::make_shared<ServerCommand>(
		number_of_threads, number_of_nodes, node_traverse_time, root_key, polling_param, sufficiency_param, 
		include_node_reports, graph_str);

	DFS::DFS_MT(command);

	std::cout << "dfs_mt done" << std::endl;

	return command->result_report.to_string();
}

void close_session() 
{
	rpc::this_session_t().post_exit();
}

void stop_server() 
{
	rpc::this_server_t().stop();
}

int main(int argc, char* argv[])
{
	rpc::server srv(9000);

	srv.bind("bfs_st", bfs_st);
	srv.bind("dfs_st", dfs_st);
	srv.bind("bfs_mt", bfs_mt);
	srv.bind("dfs_mt", dfs_mt);
	srv.bind("close_session", close_session);
	srv.bind("stop_server", stop_server);

	std::cout << "Server started on port 9000" << std::endl;

	srv.run();

	return 0;
}