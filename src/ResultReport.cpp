#include "ResultReport.h"

std::string time_to_string(std::chrono::system_clock::time_point time) 
{
	std::time_t timeT = std::chrono::system_clock::to_time_t(time);
	char buf_time[100] = "";
	std::strftime(buf_time, 100, "%H:%M:%S", std::localtime(&timeT));
	return buf_time;
}

std::string ResultReport::to_string() const
{
	std::ostringstream res_string;

	res_string << "Start time: " << time_to_string(start_time) << std::endl;
	res_string << "End time: " << time_to_string(end_time) << std::endl;

	auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	auto milis = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
	auto sec = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
	res_string << "Elapsed time: " << micros << "[us] = " << milis << "[ms] = " << sec << "[s]" << std::endl;

	for (auto&& node_result : node_results)
	{
		res_string << "Node: " << node_result.node_key << std::endl;
		res_string << "Visitor thread ID: " << node_result.visitor_id << std::endl;
		res_string << "Time visited: " << time_to_string(node_result.time_visited) << std::endl;
	}

	return res_string.str();
}
