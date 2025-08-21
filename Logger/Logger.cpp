#include "Logger.h"


Logger::Logger(
	const std::string& name,
	LogLevel::Level limit,
	const std::shared_ptr<LogFormatter>& formatter,
	const std::vector<std::shared_ptr<LogHandler>>& handlers
)
	: name(name),
	limit(limit),
	formatter(formatter),
	handlers(handlers) {
}

void Logger::Log(LogLevel::Level level, std::string filename, size_t line, const char* format, ...) {
	if (level >= limit) {
		va_list args;
		va_start(args, format);
		char buffer[128];
		vsnprintf(buffer, sizeof(buffer), format, args);
		LogMessage message(level, name, filename, line, buffer);
		Handle(message);
		va_end(args);
	}
}

SynchronousLogger::SynchronousLogger(
	const std::string& name,
	LogLevel::Level limit,
	const std::shared_ptr<LogFormatter>& formatter,
	const std::vector<std::shared_ptr<LogHandler>>& handlers
)
	: Logger(name, limit, formatter, handlers) {
}

void SynchronousLogger::Handle(const LogMessage& message) {
	std::lock_guard<std::mutex> lock(mutex);
	std::string formatted = formatter->format(message);
	for (const auto& handler : handlers) {
		handler->Log(message, formatted);
	}
}

AsynchronousLooper::AsynchronousLooper(
	const std::function<void(LogBuffer&)>& callback
)
	: running(true),
	callback(callback),
	thread(std::thread(&AsynchronousLooper::WorkerLoop, this)) {
}

AsynchronousLooper::~AsynchronousLooper() {
	Stop();
}

void AsynchronousLooper::WorkerLoop() {
	while (true) {
		{
			std::unique_lock<std::mutex> lock(mutex);
			if (!running && pbuffer.Empty()) {
				return;
			}
			consumer.wait(lock, [&]() { return !pbuffer.Empty() || !running; });
			
			cbuffer.Swap(pbuffer);
		}
		producer.notify_all();
		callback(cbuffer);
		cbuffer.Reset();
	}
}

void AsynchronousLooper::Stop() {
	if (running.exchange(false)) {
		consumer.notify_all();
		if (thread.joinable()) {
			thread.join();
		}
	}
}

void AsynchronousLooper::Push(const LogMessage& message) {
	if (running) {
		{
			std::unique_lock<std::mutex> lock(mutex);
			producer.wait(lock, [&]() { return pbuffer.WriteableSize() >= sizeof(message); });
			pbuffer.Push(message);
		}
		consumer.notify_all();
	}
}

AsynchronousLogger::AsynchronousLogger(
	const std::string& name,
	LogLevel::Level limit,
	const std::shared_ptr<LogFormatter>& formatter,
	const std::vector<std::shared_ptr<LogHandler>>& handlers
)
	: Logger(name, limit, formatter, handlers),
	lopper(std::make_shared<AsynchronousLooper>(std::bind(&AsynchronousLogger::Callback,this, std::placeholders::_1))) {
}

void AsynchronousLogger::Callback(LogBuffer& buffer) {
	while (!buffer.Empty()) {
		LogMessage message = buffer.Pop();
		std::string formatted = formatter->format(message);
		for (const auto& handler : handlers) {
			handler->Log(message, formatted);
		}
	}
}

void AsynchronousLogger::Handle(const LogMessage& message) {
	lopper->Push(message);
}