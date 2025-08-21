#include "LoggerManager.h"

LoggerManager::LoggerManager() {
	std::shared_ptr<LoggerBuilder> builder = std::make_shared<LoggerBuilder>();
	builder->BuildLoggerType(Logger::Type::Asynchronous);
	builder->BuildLoggerName("default");
	logger = builder->BuildLogger();
	loggers.insert(std::make_pair("default", logger));
}

LoggerManager& LoggerManager::GetInstance() {
	static LoggerManager instance;
	return instance;
}

void LoggerManager::AddLogger(const std::string& name, const std::shared_ptr<Logger>& logger) {
	std::unique_lock<std::mutex> lock(mutex);
	loggers.insert(std::make_pair(name, logger));
}

bool LoggerManager::HasLogger(const std::string& name) {
	std::unique_lock<std::mutex> lock(mutex);
	return loggers.contains(name);
}

std::shared_ptr<Logger> LoggerManager::GetLogger(const std::string& name) {
	std::unique_lock<std::mutex> lock(mutex);
	if (HasLogger(name)) {
		return loggers[name];
	}
	return std::shared_ptr<Logger>();
}

std::shared_ptr<Logger> LoggerManager::GetDefaultLogger() {
	std::unique_lock<std::mutex> lock(mutex);
	return logger;
}