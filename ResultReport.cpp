#include <sstream>
#include "ResultReport.h"
#include "ServerCommand.h"

std::string ResultReport::to_xml_string() const
{
	std::ostringstream xml_string;

	xml_string << "<startTime>" << std::chrono::system_clock::to_time_t(start_time) << "</startTime>";
	xml_string << "<endTime>" << std::chrono::system_clock::to_time_t(end_time) << "</endTime>";
	xml_string << "<elapsedTime>" << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "[ms]</elapsedTime>";
	
	for(auto&& node_result : node_results)
	{
		xml_string << "<node>" << node_result->node_key << "</node>";
		xml_string << "<visitorThreadId>" << node_result->visitor_id << "</visitorThreadId>";
		xml_string << "<timeVisited>" << std::chrono::system_clock::to_time_t(node_result->time_visited) << "</timeVisited>";
	}
	
	return xml_string.str();
}