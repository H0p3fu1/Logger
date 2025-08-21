#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "LogMessage.h"

class LogHandler {
public:
	virtual ~LogHandler() = default;

public:
	virtual void Log(const LogMessage& message, const std::string& formatted) = 0;
	virtual void Log(const std::string& formatted) = 0;
};

class ConsoleLogHandler : public LogHandler {
private:
	void SetConsoleColor(LogLevel::Level level);
	void ResetConsoleColor();

public:
	void Log(const LogMessage& message, const std::string& formatted) override;
	void Log(const std::string& formatted) override;
};

class FileLogHandler : public LogHandler {
private:
	std::ofstream file;

public:
	FileLogHandler(const std::string& filename);
	~FileLogHandler();

public:
	void Log(const LogMessage& message, const std::string& formatted) override;
	void Log(const std::string& formatted) override;
};

class LogHandlerFactory {
public:
	template <typename LogHandlerType, typename... Args>
	static std::shared_ptr<LogHandler> CreateLogHandler(Args &&...args) {
		return std::make_shared<LogHandlerType>(std::forward<Args>(args)...);
	}
};


