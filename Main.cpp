#include "GraphGenerator.h"
#include "SingleThreadGraphAlgorithms.h"
#include "CppThreadsGraphAlgorithms.h"
#include <iostream>
using namespace std;

int main() 
{
	GraphGenerator gen = GraphGenerator::getInstance();
	GraphNode* root = gen.generateGraph(GraphType::CompleteGraph, 10);
	
	
	//SingleThreadGraphAlgorithms* ga = new SingleThreadGraphAlgorithms(root, 10);
	//ga->BFS();
	
	
	Logger::getInstance()->logMessage("Start");


	CppThreadsGraphAlgorithms* cpa = new CppThreadsGraphAlgorithms(root, 10);
	cpa->BFS();


	ThreadPool::getInstance()->joinThreadsFromPool();
	Logger::getInstance()->joinThread();
	
	return 0;
}