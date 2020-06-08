#pragma once

template <typename T>
class DataNode
{
public:
	T* data;
	DataNode* next;
	DataNode() 
	{
		data = NULL;
		next = NULL;
	};
	~DataNode() 
	{
		delete data;
		delete next;
	};
};

