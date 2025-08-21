#include "LogLevel.h"

std::string LogLevel::ToString(LogLevel::Level level) {
	switch (level) {
	case LogLevel::Level::Debug: return "DEBUG";
	case LogLevel::Level::Information: return "INFOMATION";
	case LogLevel::Level::Warning: return "WARNING";
	case LogLevel::Level::Error: return "ERROR";
	case LogLevel::Level::Fatal: return "FATAL";
	default: return "UNKNOWN";
	}
}