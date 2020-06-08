#include "GraphGenerator.h"
#include "SingleThreadGraphAlgorithms.h"
#include "CppThreadsGraphAlgorithms.h"
#include <iostream>
using namespace std;

int main() 
{
	GraphGenerator gen = GraphGenerator::get_instance();
	GraphNode* root = gen.generate_graph(GraphType::CompleteGraph, 10);
	
	
	//SingleThreadGraphAlgorithms* ga = new SingleThreadGraphAlgorithms(root, 10);
	//ga->BFS();
	
	
	Logger::get_instance()->log_message("Start");


	CppThreadsGraphAlgorithms* cpa = new CppThreadsGraphAlgorithms(root, 10);
	cpa->BFS();


	ThreadPool::getInstance()->joinThreadsFromPool();
	Logger::get_instance()->log_message("Done");
	
	return 0;
}