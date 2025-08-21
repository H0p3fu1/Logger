#include "LoggerManager.h"

namespace Log {
	std::shared_ptr<Logger> GetlLogger(const std::string& name) {
		return LoggerManager::GetInstance().GetLogger(name);
	}

	std::shared_ptr<Logger> GetDefaultLogger() {
		return LoggerManager::GetInstance().GetDefaultLogger();
	}

	// 使用宏函数对日志器的接口代理
#define Debug(format, ...) Log(LogLevel::Level::Debug, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Information(format, ...) Log(LogLevel::Level::Information, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Warning(format, ...) Log(LogLevel::Level::Warning, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Error(format, ...) Log(LogLevel::Level::Error, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Fatal(format, ...) Log(LogLevel::Level::Fatal, __FILE__, __LINE__, format, ##__VA_ARGS__)

// 提供宏函数 提供默认日志器进行日志的标准输出打印
#define DEBUG(format,...) GetDefaultLogger()->Debug(format,##__VA_ARGS__)
#define INFORMATION(format,...) GetDefaultLogger()->Information(format,##__VA_ARGS__)
#define WARNING(format,...) GetDefaultLogger()->Warning(format,##__VA_ARGS__)
#define ERROR(format,...) GetDefaultLogger()->Error(format,##__VA_ARGS__)
#define FATAL(format,...) GetDefaultLogger()->Fatal(format,##__VA_ARGS__)
}

#include <chrono>
void Bench(std::shared_ptr<Logger>& logger, size_t thread_count, size_t message_count, size_t message_length) {
	std::string message(message_length, 'M');
	std::vector<std::thread> threads;
	std::cout << "输入线程数量: " << thread_count << "  输出日志数量: " << message_count << "  输出总日志大小: " << message_count * message_length / 1024 / 1024 << "MB" << std::endl;
	std::vector<double> cost_time(thread_count);
	size_t message_count_per_thread = message_count / thread_count; 
	for (int i = 0; i < thread_count; i++) {
		threads.emplace_back([&, i]() {
			auto start = std::chrono::high_resolution_clock::now();
			for (size_t j = 0; j < message_count_per_thread; j++) {
				logger->Information("%s", message.c_str());
			}
			auto end = std::chrono::high_resolution_clock::now();
			auto cost = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
			cost_time[i] = cost.count();
			std::cout << "线程" << i << " 耗时：" << cost_time[i] << "s" << std::endl;
			});
	}
	for (auto& thread : threads) {
		thread.join();
	}

	double max_cost = 0;
	for (auto cost : cost_time) {
		max_cost = (max_cost < cost ? cost : max_cost);
	}
	std::cout << "总消耗时间: " << max_cost << "s" << std::endl;
	std::cout << "平均每秒输出: " << (size_t)(message_count / max_cost) << "条日志" << std::endl;
	std::cout << "平均每秒输出: " << (size_t)(message_length * message_count / max_cost / 1024 / 1024) << "MB" << std::endl;
}

void SynchronousBench() {
	std::shared_ptr<LoggerBuilder> builder = std::make_shared<LoggerBuilder>();
	builder->BuildLoggerName("SynchronousLogger");
	builder->BuildLoggerType(Logger::Type::Synchronous);
	builder->BuildLogFormatter("%m");
	builder->BuildLogHandler<FileLogHandler>("logs/synchronous.log");
	std::shared_ptr<Logger> logger = builder->BuildLogger();
	Bench(logger, 1, 1000000, 100);
	Bench(logger, 3, 1000000, 100);
}

void AsynchronousBench() {
	std::shared_ptr<LoggerBuilder> builder = std::make_shared<LoggerBuilder>();
	builder->BuildLoggerName("AsynchronousLogger");
	builder->BuildLoggerType(Logger::Type::Asynchronous);
	builder->BuildLogFormatter("%m");
	builder->BuildLogHandler<FileLogHandler>("logs/asynchronous.log");
	std::shared_ptr<Logger> logger = builder->BuildLogger();
	Bench(logger, 1, 1000000, 100);
	Bench(logger, 3, 1000000, 100);
}

int main() {
	SynchronousBench();
	AsynchronousBench();
	return 0;
}