#pragma once
#include <string>
#include <thread>
#include "LogLevel.h"

class LogMessage {
private:
	size_t time;			// ʱ��
	std::thread::id tid;	// �߳�ID
	LogLevel::Level level;	// ��־�ȼ�
	std::string name;		// ��־������
	std::string filename;	// �ļ���
	size_t line;			// �к�
	std::string context;	// ��־��Ϣ

public:
	LogMessage() = default;
	LogMessage(
		LogLevel::Level level, 
		const std::string& name,
		const std::string& filename,
		size_t line,
		const std::string& context
	);

public:
	size_t GetTime() const;
	std::thread::id GetThreadId() const;
	LogLevel::Level GetLevel() const;
	std::string GetName() const;
	std::string GetFilename() const;
	size_t GetLine() const;
	std::string GetContext() const;
};