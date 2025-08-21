#pragma once
#include <stdarg.h>
#include <mutex>
#include <memory>
#include <functional>
#include "LogLevel.h"
#include "LogFormatter.h"
#include "LogHandler.h"
#include "LogMessage.h"
#include "LogBuffer.h"

class Logger {
public:
	enum class Type {
		Synchronous,
		Asynchronous
	};

protected:
	std::mutex mutex;
	std::atomic<LogLevel::Level> limit;
	std::string name;
	std::shared_ptr<LogFormatter> formatter;
	std::vector<std::shared_ptr<LogHandler>> handlers;

public:
	Logger(const std::string& name, LogLevel::Level limit, const std::shared_ptr<LogFormatter>& formatter, const std::vector<std::shared_ptr<LogHandler>>& handlers);

public:
	void Log(LogLevel::Level level, std::string filename, size_t line, const char* format, ...);
	virtual void Handle(const LogMessage& message) = 0;
};

class SynchronousLogger :public Logger {
public:
	SynchronousLogger(const std::string& name, LogLevel::Level limit, const std::shared_ptr<LogFormatter>& formatter, const std::vector<std::shared_ptr<LogHandler>>& handlers);

public:
	void Handle(const LogMessage& message) override;
};

class AsynchronousLooper {
private:
	std::function<void(LogBuffer&)> callback;

	std::mutex mutex;
	std::atomic<bool> running;          // �����첽�߳��Ƿ�������л�ȫ�˳��������첽�߳����������е�һ�����Ʊ�־
	LogBuffer pbuffer;                  // ����������
	std::condition_variable producer;	// ��������������
	LogBuffer cbuffer;                  // ���ѻ�����
	std::condition_variable consumer;	// ��������������
	std::thread thread;                 // �����߳�,������Ȼֻ��һ���̵߳��Ǻ������������̳߳صȴ���������չ�ԣ�����һ����־ϵͳ�Ͳ���Ҫ����߳���ʵ��

public:
	AsynchronousLooper(const std::function<void(LogBuffer&)>& callback);
	~AsynchronousLooper();

private:
	void WorkerLoop();

public:
	void Stop();
	void Push(const LogMessage& message);
};

class AsynchronousLogger :public Logger {
private:
	std::shared_ptr<AsynchronousLooper> lopper;

public:
	AsynchronousLogger(
		const std::string& name,
		LogLevel::Level limit,
		const std::shared_ptr<LogFormatter>& formatter,
		const std::vector<std::shared_ptr<LogHandler>>& handlers
	);

public:
	void Callback(LogBuffer& message);
	void Handle(const LogMessage& message) override;
};
