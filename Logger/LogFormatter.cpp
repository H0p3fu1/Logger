#include "LogFormatter.h"

DateTimeFormatItem::DateTimeFormatItem(const std::string& pattern)
	:pattern(pattern) {
}

std::string DateTimeFormatItem::format(const LogMessage& message) {
	std::time_t time = std::time(nullptr);
	struct tm t;
#if defined(_WIN32)
	localtime_s(&t, &time);
#else
	localtime_r(&time, &t);
#endif
	char buffer[128];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &t);
	return std::string(buffer);
}

std::string ThreadIdFormatItem::format(const LogMessage& message) {
	std::stringstream ss;
	ss << message.GetThreadId();
	return ss.str();
}

std::string LogLevelFormatItem::format(const LogMessage& message) {
	return LogLevel::ToString(message.GetLevel());
}

std::string NameFormatItem::format(const LogMessage& message) {
	return message.GetName();
}

std::string FilenameFormatItem::format(const LogMessage& message) {
	return message.GetFilename();
}

std::string LineFormatItem::format(const LogMessage& message) {
	return std::to_string(message.GetLine());
}

std::string ContextFormatItem::format(const LogMessage& message) {
	return message.GetContext();
}

StringFormatItem::StringFormatItem(const std::string& string)
	:string(string) {
}

std::string StringFormatItem::format(const LogMessage& message) {
	return string;
}

LogFormatter::LogFormatter(const std::string& pattern)
	:pattern(pattern) {
	ParsePattern(pattern);
}

void LogFormatter::ParsePattern(const std::string& pattern) {
	for (size_t i = 0; i < pattern.size();) {
		if (pattern[i] == '%') {
			if (i + 1 < pattern.size()) {
				char c = pattern[i + 1];
				switch (c) {
				case 'd': {
					// 解析 %d{格式}
					size_t start = pattern.find('{', i);
					size_t end = pattern.find('}', start);
					std::string format = "%Y-%m-%d %H:%M:%S";
					if (start != std::string::npos && end != std::string::npos) {
						format = pattern.substr(start + 1, end - start - 1);
					}
					items.push_back(std::make_shared<DateTimeFormatItem>(format));
					i = end + 1;
				} break;
				case 't': items.push_back(std::make_shared<ThreadIdFormatItem>()); i += 2; break;
				case 'l': items.push_back(std::make_shared<LogLevelFormatItem>()); i += 2; break;
				case 'n': items.push_back(std::make_shared<NameFormatItem>()); i += 2; break;
				case 'f': items.push_back(std::make_shared<FilenameFormatItem>()); i += 2; break;
				case 'i': items.push_back(std::make_shared<LineFormatItem>()); i += 2; break;
				case 'm': items.push_back(std::make_shared<ContextFormatItem>()); i += 2; break;
				default: items.push_back(std::make_shared<StringFormatItem>(std::string(1, c))); i += 2;
				}
			}
			else {
				items.push_back(std::make_unique<StringFormatItem>(std::string(1, pattern[i])));
				i++;
			}
		}
		else {
			// 普通字符直接输出
			items.push_back(std::make_unique<StringFormatItem>(std::string(1, pattern[i])));
			i++;
		}
	}
}

std::string LogFormatter::format(const LogMessage& message) {
	std::stringstream ss;
	for (const auto& item : items) {
		ss << item->format(message);
	}
	return ss.str();
}