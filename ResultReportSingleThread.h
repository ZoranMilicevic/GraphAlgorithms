#pragma once


#include <chrono>
#include <vector>
#include <string>
#include <vector>
#include "ResultReport.h"
#include "NodeResultReport.h"


class ResultReportSingleThread : public ResultReport
{
public:
	virtual ~ResultReportSingleThread()
	{
		for (auto&& node_result : node_results)
			delete node_result;
	}

	static ResultReport* create_new_report()
	{
		delete ResultReport::cur_repport;
		ResultReport::cur_repport = new ResultReportSingleThread();
		return cur_repport;
	}

	void add_node_result_report(NodeResultReport* node_result_report)
	{
		node_results.push_back(node_result_report);
	}

private:
	std::vector<NodeResultReport*> node_results;
};