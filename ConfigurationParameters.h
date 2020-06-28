#pragma once

#include <string>
#include <map>
#include <thread>
#include <chrono>

class ConfigurationParameters 
{
public:	
	inline static int wait_time = 100;
	inline static std::chrono::microseconds cond_var_wait_time{ wait_time };
};

