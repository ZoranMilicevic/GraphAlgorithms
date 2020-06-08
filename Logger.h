#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <fstream>
#include <iostream>
#include <thread>
#include <atomic>
#include "DataQueueSingleThread.h"
#include "DataQueueCppThreads.h"

class Logger 
{
public:
	static Logger* get_instance() 
	{
		static Logger* instance = new Logger();
		return instance;
	}

	void log_message(const std::string& message) 
	{
		message_queue->push(message);
	}

	void logging_procedure() 
	{
		while(!done)
		{
			std::string* s = message_queue->try_pop();
			if (s == NULL)
				continue;
			std::cout << *s << std::endl;
			log_file << *s << std::endl;
		}
	}

	void stop_logger()
	{
		done = true;
	}

	void join_logger_thread()
	{
		if (worker_thread.joinable())
			worker_thread.join();
	}

	virtual ~Logger()
	{
		std::string* s = NULL;
		while ((s = message_queue->pop()) != NULL)
		{
			log_file << *s << std::endl;
		}
		done = true;
	};

private:
	Logger()
		:message_queue(new DataQueueCppThreads<std::string>()), 
		worker_thread(&Logger::logging_procedure, this),
		log_file(ConfigurationParameters::log_file_name, std::ofstream::out),
		done(false){}


	DataQueueCppThreads<std::string>* message_queue;
	std::thread worker_thread;
	std::ofstream log_file;
	std::atomic<bool> done;
};

