#pragma once

#include <string>
#include <map>
#include <thread>
#include <chrono>
#include "GraphType.h"

class ConfigurationParameters 
{
public:	
	inline static const int traverse_time = 1000;
	inline static const std::chrono::microseconds node_traverse_time{ traverse_time };

	inline static const int wait_time = 100;
	inline static const std::chrono::microseconds cond_var_wait_time{ wait_time };

	inline static const std::string py_module_name = "GenerateGraphModule";
	inline static const std::map<GraphType, std::string> py_method_names = 
	{
		{GraphType::CompleteGraph, "generateCompleteGraph"}
	};
	inline static unsigned const thread_count = 1;//std::thread::hardware_concurrency();

};

