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
	std::atomic<bool> running;          // 控制异步线程是否继续运行或安全退出，它是异步线程生命周期中的一个控制标志
	LogBuffer pbuffer;                  // 生产缓冲区
	std::condition_variable producer;	// 生产者条件变量
	LogBuffer cbuffer;                  // 消费缓冲区
	std::condition_variable consumer;	// 消费者条件变量
	std::thread thread;                 // 工作线程,这里虽然只有一个线程但是后续可以引入线程池等待，符合扩展性，但是一个日志系统就不需要多个线程来实现

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
