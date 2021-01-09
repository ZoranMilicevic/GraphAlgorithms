#include <iostream>
#include <memory>
#include "rpc/server.h"
#include "rpc/this_server.h"
#include "rpc/this_session.h"
#include "ServerCommand.h"
#include "BFS_ST_Command.h"
#include "BFS_MT_Command.h"
#include "DFS_ST_Command.h"
#include "DFS_MT_Command.h"
#include "MST_ST_Command.h"
#include "MST_MT_Command.h"

#define DEFAULT_PORT 9000

std::string bfs_st(const std::string& graph_str, unsigned root_key, unsigned node_traverse_time, bool include_node_reports)
{
	BFS_ST_Command command(graph_str, root_key, node_traverse_time, include_node_reports);

	std::cout << "bfs_st called" << std::endl;
	command.do_command();
	std::cout << "bfs_st done" << std::endl;

	return command.get_result_string();
}

std::string dfs_st(const std::string& graph_str, unsigned root_key, unsigned node_traverse_time, bool include_node_reports)
{
	DFS_ST_Command command(graph_str, root_key, node_traverse_time, include_node_reports);

	std::cout << "dfs_st called" << std::endl;
	command.do_command();
	std::cout << "dfs_st done" << std::endl;

	return command.get_result_string();
}

std::string bfs_mt(const std::string& graph_str, unsigned root_key, unsigned node_traverse_time, bool include_node_reports,
	unsigned number_of_threads)
{
	if (number_of_threads == 0 || number_of_threads > std::thread::hardware_concurrency())
		number_of_threads = std::thread::hardware_concurrency();

	BFS_MT_Command command(graph_str, root_key, node_traverse_time, include_node_reports, number_of_threads);

	std::cout << "bfs_mt called" << std::endl;
	command.do_command();
	std::cout << "bfs_mt done" << std::endl;

	return command.get_result_string();
}

std::string dfs_mt(const std::string& graph_str, unsigned root_key, unsigned node_traverse_time, bool include_node_reports,
	unsigned number_of_threads, unsigned polling_param, unsigned sufficiency_param)
{
	if (number_of_threads == 0 || number_of_threads > std::thread::hardware_concurrency())
		number_of_threads = std::thread::hardware_concurrency();

	DFS_MT_Command command(graph_str, root_key, node_traverse_time, include_node_reports, number_of_threads, polling_param, sufficiency_param);

	std::cout << "bfs_mt called" << std::endl;
	command.do_command();
	std::cout << "bfs_mt done" << std::endl;

	return command.get_result_string();
}

std::string mst_st(const std::string& graph_str, unsigned root_key, unsigned node_traverse_time, bool include_node_reports)
{
	MST_ST_Command command(graph_str, root_key, node_traverse_time, include_node_reports);

	std::cout << "mst_st called" << std::endl;
	command.do_command();
	std::cout << "mst_st done" << std::endl;

	return command.get_result_string();
}

std::string mst_mt(const std::string& graph_str, unsigned root_key, unsigned node_traverse_time, bool include_node_reports,
	unsigned number_of_threads)
{
	if (number_of_threads == 0 || number_of_threads > std::thread::hardware_concurrency())
		number_of_threads = std::thread::hardware_concurrency();

	MST_MT_Command command(graph_str, root_key, node_traverse_time, include_node_reports, number_of_threads);

	std::cout << "mst_mt called" << std::endl;
	command.do_command();
	std::cout << "mst_mt done" << std::endl;

	return command.get_result_string();
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
	static int port = DEFAULT_PORT;
	if (argc > 2 && strcmp(argv[1], "-port"))
		port = atoi(argv[2]);

	rpc::server srv(port);

	srv.bind("bfs_st", bfs_st);
	srv.bind("dfs_st", dfs_st);
	srv.bind("bfs_mt", bfs_mt);
	srv.bind("dfs_mt", dfs_mt);
	srv.bind("mst_st", mst_st);
	srv.bind("mst_mt", mst_mt);
	srv.bind("close_session", close_session);
	srv.bind("stop_server", stop_server);

	std::cout << "Server started on port " << port << std::endl;

	srv.run();

	return 0;
}