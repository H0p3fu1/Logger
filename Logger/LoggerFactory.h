#pragma once
#include <memory>
#include "Logger.h"

class LoggerFactory {
public:
	template<typename LoggerType, typename... Args>
	static std::shared_ptr<Logger> CreateLogger(Args&&... args) {
		return std::make_shared<LoggerType>(std::forward<Args>(args)...);
	}
};