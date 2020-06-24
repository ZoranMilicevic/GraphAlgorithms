#pragma once

#include <chrono>
#include <vector>
#include <string>
#include "ResultReport.h"
#include "NodeResultReport.h"
#include "DataQueueCppThreads.h"

class ResultReportCppThreads : public ResultReport
{
public:
	static ResultReport* create_new_report()
	{
		delete ResultReport::cur_repport;
		ResultReport::cur_repport = new ResultReportCppThreads();
		return cur_repport;
	}

	void add_node_result_report(NodeResultReport* node_result_report)
	{
		node_results.push(node_result_report);
	}

private:
	DataQueueCppThreads<NodeResultReport*> node_results;
};