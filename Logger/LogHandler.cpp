#include "LogHandler.h"
#include "LogLevel.h"
#include <windows.h>
#include "FileSystem.h"

void ConsoleLogHandler::SetConsoleColor(LogLevel::Level level) {
	switch (level) {
	case LogLevel::Level::Debug: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY); break;
	case LogLevel::Level::Information: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY); break;
	case LogLevel::Level::Warning: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); break;
	case LogLevel::Level::Error: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY); break;
	case LogLevel::Level::Fatal: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY); break;
	default: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); break;
	}
}

void ConsoleLogHandler::ResetConsoleColor() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void ConsoleLogHandler::Log(const LogMessage& message,const std::string& formatted) {
	SetConsoleColor(message.GetLevel());
	std::cout << formatted << std::endl;
	ResetConsoleColor();
}

void ConsoleLogHandler::Log(const std::string& formatted) {
	std::cout << formatted << std::endl;
}

FileLogHandler::FileLogHandler(const std::string& filename) {
	FileSystem::CreateDirectories(FileSystem::GetParentDirectory(filename));
	file.open(filename, std::ios::app);
}

FileLogHandler::~FileLogHandler() {
	if (file.is_open()) {
		file.close();
	}
}

void FileLogHandler::Log(const LogMessage& message, const std::string& formatted) {
	file << formatted << std::endl;
}

void FileLogHandler::Log(const std::string& formatted) {
	file << formatted << std::endl;
}