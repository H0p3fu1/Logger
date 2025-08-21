#include "LogMessage.h"
#include <ctime>

LogMessage::LogMessage(
	LogLevel::Level level,
	const std::string& name,
	const std::string& filename,
	size_t line,
	const std::string& context
)
	:
	time(static_cast<size_t>(std::time(nullptr))),
	tid(std::this_thread::get_id()),
	level(level),
	name(name),
	filename(filename),
	line(line),
	context(context) {
}

size_t LogMessage::GetTime() const {
	return time;
}

std::thread::id LogMessage::GetThreadId() const {
	return tid;
}

LogLevel::Level LogMessage::GetLevel() const {
	return level;
}

std::string LogMessage::GetName() const {
	return name;
}

std::string LogMessage::GetFilename() const {
	return filename;
}

size_t LogMessage::GetLine() const {
	return line;
}

std::string LogMessage::GetContext() const {
	return context;
}

