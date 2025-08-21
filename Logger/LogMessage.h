#pragma once
#include <string>
#include <thread>
#include "LogLevel.h"

class LogMessage {
private:
	size_t time;			// 时间
	std::thread::id tid;	// 线程ID
	LogLevel::Level level;	// 日志等级
	std::string name;		// 日志器名称
	std::string filename;	// 文件名
	size_t line;			// 行号
	std::string context;	// 日志消息

public:
	LogMessage() = default;
	LogMessage(
		LogLevel::Level level, 
		const std::string& name,
		const std::string& filename,
		size_t line,
		const std::string& context
	);

public:
	size_t GetTime() const;
	std::thread::id GetThreadId() const;
	LogLevel::Level GetLevel() const;
	std::string GetName() const;
	std::string GetFilename() const;
	size_t GetLine() const;
	std::string GetContext() const;
};