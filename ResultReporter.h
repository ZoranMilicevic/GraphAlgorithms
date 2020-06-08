#pragma once

#include <atomic>
#include <thread>
#include "ResultReport.h"

class ResultReporter
{
	static ResultReporter* get_instance()
	{
		static ResultReporter* instance = new ResultReporter();
	}

	void report()
	{
		
	}

	void report_procedure()
	{
	
	}

	virtual ~ResultReporter() {}

private:
	ResultReporter(){}
	
	ResultReport* curent_report;
	std::atomic<int> done;
	std::thread worker_thread;

};