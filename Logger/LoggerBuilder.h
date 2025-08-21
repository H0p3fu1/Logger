#pragma once
#include <string>
#include "LogLevel.h"
#include "LogFormatter.h"
#include "LogHandler.h"
#include "Logger.h"

class LoggerBuilder {
protected:
	Logger::Type type;
	std::atomic<LogLevel::Level> limit;
	std::string name;
	std::shared_ptr<LogFormatter> formatter;
	std::vector<std::shared_ptr<LogHandler>> handlers;

public:
	void BuildLoggerType(Logger::Type type);
	void BuildLoggerLimit(LogLevel::Level limit);
	void BuildLoggerName(const std::string& name);
	void BuildLogFormatter(const std::string& pattern);

	template<typename LogHandlerType, typename ...Args>
	void BuildLogHandler(Args&& ...args) {
		handlers.push_back(LogHandlerFactory::CreateLogHandler<LogHandlerType>(std::forward<Args>(args)...));
	}

	std::shared_ptr<Logger> BuildLogger();
};
