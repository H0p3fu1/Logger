#pragma once
#include <vector>
#include "LogMessage.h"

class LogBuffer {
private:
	std::vector<LogMessage> buffer;		// ������
	size_t read;						// buffer�ж�дλ������
	size_t write;						// buffer�п�дλ������

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
	std::vector<char> buffer;			// ������
	size_t read;						// buffer�ж�дλ������
	size_t write;						// buffer�п�дλ������

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