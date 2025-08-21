#pragma once
#include <vector>
#include "LogMessage.h"

class LogBuffer {
private:
	std::vector<LogMessage> buffer;		// 缓冲区
	size_t read;						// buffer中读写位置索引
	size_t write;						// buffer中可写位置索引

public:
	LogBuffer();

public:
	LogMessage* Begin();
	bool Empty();
	size_t ReadableSize();
	size_t WriteableSize();
	void Reset();
	void Swap(LogBuffer& buffer);
	void Push(const LogMessage& message);
	LogMessage Pop();
};

class LogBuffer2 {
private:
	std::vector<char> buffer;			// 缓冲区
	size_t read;						// buffer中读写位置索引
	size_t write;						// buffer中可写位置索引

public:
	LogBuffer2();

public:
	char* Begin();
	bool Empty();
	size_t ReadableSize();
	size_t WriteableSize();
	void Reset();
	void Swap(LogBuffer2& buffer);
	void Push(const std::string& message);
	void Pop(size_t length);
};