#include <sstream>
#include "ResultReport.h"
#include "ServerCommand.h"
#include "GraphAlgorithms.h"

void ResultReport::fill_from_server_command(ServerCommand* sc)
{
	this->algorithm = graph_algorithm_to_string[(int)sc->algorithm];
	this->number_of_worker_threads = sc->number_of_threads;
	this->number_of_nodes = sc->number_of_nodes;
	this->traverse_time = sc->node_traverse_time;
	this->wait_time = sc->cond_var_wait_time;
	this->root_key = sc->root_key;
	this->graph_type = sc->graph_type;

	node_results = new NodeResultReport * [number_of_nodes];
}

//todo: make better
std::string ResultReport::to_xml_string()
{
	std::ostringstream xml_string;
	xml_string << "<algorithm>" << algorithm << "</algorithm>";
	xml_string << "<numberOfThreads>" << number_of_worker_threads << "</numberOfThreads>";
	xml_string << "<numberOfNodes>" << number_of_nodes << "</numberOfNodes>";
	xml_string << "<nodeTraverseTime>" << traverse_time << "</nodeTraverseTime>";
	xml_string << "<condVarWaitTime>" << wait_time << "</condVarWaitTime>";
	xml_string << "<rootKey>" << root_key << "</rootKey>";
	xml_string << "<graphType>" << graph_type << "</graphType>";

	xml_string << "<startTime>" << std::chrono::system_clock::to_time_t(start_time) << "</startTime>";
	xml_string << "<endTime>" << std::chrono::system_clock::to_time_t(end_time) << "</endTime>";
	xml_string << "<elapsedTime>" << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "[ms]</elapsedTime>";
	
	for(int i = 0; i<number_of_nodes; i++)
	{
		NodeResultReport* node_result = node_results[i];
		xml_string << "<node>" << node_result->node_key << "</node>";
		xml_string << "<visitorThreadId>" << node_result->visitor_id << "</visitorThreadId>";
		xml_string << "<timeVisited>" << std::chrono::system_clock::to_time_t(node_result->time_visited) << "</timeVisited>";
	}
	
	return xml_string.str();
}