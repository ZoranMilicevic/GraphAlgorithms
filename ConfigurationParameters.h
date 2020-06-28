#pragma once

#include <string>
#include <map>
#include <thread>
#include <chrono>

class ConfigurationParameters 
{
public:	
	inline static const std::string log_file_name = "C:\\Users\\Zoran Milicevic\\source\\repos\\OptimisationOfGraphAlgorithms\\GraphAlgorithms\\log.txt";

	inline static int traverse_time = 1000;
	inline static std::chrono::microseconds node_traverse_time{ traverse_time };

	inline static int wait_time = 100;
	inline static std::chrono::microseconds cond_var_wait_time{ wait_time };
};

