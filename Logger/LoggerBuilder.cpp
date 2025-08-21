#include "LoggerBuilder.h"

void LoggerBuilder::BuildLoggerType(Logger::Type type) {
	this->type = type;
}

void LoggerBuilder::BuildLoggerLimit(LogLevel::Level limit) {
	this->limit = limit;
}

void LoggerBuilder::BuildLoggerName(const std::string& name) {
	this->name = name;
}

void LoggerBuilder::BuildLogFormatter(const std::string& pattern) {
	this->formatter = std::make_shared<LogFormatter>(pattern);
}

std::shared_ptr<Logger> LoggerBuilder::BuildLogger() {
	if (!formatter) {
		formatter = std::make_shared<LogFormatter>();
	}

	if (handlers.empty()) {
		handlers.push_back(std::make_shared<ConsoleLogHandler>());
	}

	std::shared_ptr<Logger> logger;
	switch (type)
	{
	case Logger::Type::Synchronous:
		logger = std::make_shared<SynchronousLogger>(name, limit, formatter, handlers);
		break;
	case Logger::Type::Asynchronous:
		logger = std::make_shared<AsynchronousLogger>(name, limit, formatter, handlers);
		break;
	default:
		logger = std::make_shared<SynchronousLogger>(name, limit, formatter, handlers);
		break;
	}
	return logger;
}