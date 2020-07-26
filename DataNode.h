#pragma once

#include <memory>

template <typename T>
class DataNode
{
public:
	std::shared_ptr<T> data;
	std::unique_ptr<DataNode> next;
	DataNode() 
	{
		data = nullptr;
		next = nullptr;
	};
	~DataNode() {};
};

