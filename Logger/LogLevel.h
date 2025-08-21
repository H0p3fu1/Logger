#pragma once
#include <string>

class LogLevel {
public:
	enum class Level {
		Debug,
		Information,
		Warning,
		Error,
		Fatal,
	};

public:
	static std::string ToString(LogLevel::Level level);
};