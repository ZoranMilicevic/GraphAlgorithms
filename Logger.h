#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <iostream>
#include <thread>
#include "DataQueueSingleThread.h"
#include "DataQueueCppThreads.h"

class Logger 
{
public:
	static Logger* getInstance() 
	{
		static Logger* instance = new Logger();
		return instance;
	}

	void logMessage(const std::string& message) 
	{
		message_queue->push(message);
	}

	void loggingProcedure() 
	{
		while(1)
		{
			std::string* s = message_queue->pop();
			if (s == NULL) continue;
			std::cout << *s << std::endl;
		}
	}

	void joinThread()
	{
		if (t.joinable())
			t.join();
	}

private:
	Logger():message_queue(new DataQueueCppThreads<std::string>()), t(&Logger::loggingProcedure, this){}
	virtual ~Logger() 
	{
		std::string* s = NULL;
		while((s = message_queue->pop())!=NULL)
		{
			std::cout << *s << std::endl;
		}
	};

	std::mutex m;
	DataQueueCppThreads<std::string>* message_queue;
	std::thread t;
};

