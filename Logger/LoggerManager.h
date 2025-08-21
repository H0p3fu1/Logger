#pragma once
#include <mutex>
#include <unordered_map>
#include <string>
#include <memory>
#include "Logger.h"
#include "LoggerBuilder.h"

class LoggerManager {
private:
	std::mutex mutex;
	std::unordered_map<std::string, std::shared_ptr<Logger>> loggers;
	std::shared_ptr<Logger> logger;

private:
	LoggerManager();
	~LoggerManager() = default;

	LoggerManager(const LoggerManager&) = delete;
	LoggerManager& operator=(const LoggerManager&) = delete;
	LoggerManager(LoggerManager&&) = delete;
	LoggerManager& operator=(LoggerManager&&) = delete;

public:
	static LoggerManager& GetInstance();
	void AddLogger(const std::string& name, const std::shared_ptr<Logger>& logger);
	bool HasLogger(const std::string& name);
	std::shared_ptr<Logger> GetLogger(const std::string& name);
	std::shared_ptr<Logger> GetDefaultLogger();
};

