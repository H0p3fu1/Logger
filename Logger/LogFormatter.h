#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include "LogMessage.h"

class FormatItem {
public:
	virtual std::string format(const LogMessage& message) = 0;
};

class DateTimeFormatItem :public FormatItem {
private:
	std::string pattern;

public:
	DateTimeFormatItem(const std::string& pattern = "%Y-%m-%d %H:%M:%S");

public:
	std::string format(const LogMessage& message) override;
};

class ThreadIdFormatItem :public FormatItem {
public:
	std::string format(const LogMessage& message) override;
};

class LogLevelFormatItem :public FormatItem {
public:
	std::string format(const LogMessage& message) override;
};

class NameFormatItem :public FormatItem {
public:
	std::string format(const LogMessage& message) override;
};

class FilenameFormatItem :public FormatItem {
public:
	std::string format(const LogMessage& message) override;
};

class LineFormatItem :public FormatItem {
public:
	std::string format(const LogMessage& message) override;
};

class ContextFormatItem :public FormatItem {
public:
	std::string format(const LogMessage& message) override;
};

class StringFormatItem :public FormatItem {
private:
	std::string string;

public:
	StringFormatItem(const std::string& string);

public:
	std::string format(const LogMessage& message) override;
};

class LogFormatter {
private:
	//  %d 日期
	//  %t 线程id
	//  %l 日志级别
	//  %n 日志器名称
	//  %f 文件名
	//  %i 行号
	//  %m 日志消息
	std::string pattern;
	std::vector<std::shared_ptr<FormatItem>> items;

public:
	LogFormatter(const std::string& pattern = "[%d{%Y-%m-%d %H:%M:%S}][%t][%l][%n][%f:%i] %m");

private:
	void ParsePattern(const std::string& pattern);

public:
	std::string format(const LogMessage& message);
};



