#include "LogBuffer.h"

LogBuffer::LogBuffer()
	: buffer(4 * 1024), read(0), write(0) {
}

LogMessage* LogBuffer::Begin() {
	return static_cast<LogMessage*>(&buffer[read]);
}

bool LogBuffer::Empty() {
	return read == write;
}

size_t LogBuffer::ReadableSize() {
	return write - read;
}

size_t LogBuffer::WriteableSize() {
	return buffer.size() - write;
}

void LogBuffer::Reset() {
	read = write = 0;
}

void LogBuffer::Swap(LogBuffer& buffer) {
	this->buffer.swap(buffer.buffer);
	std::swap(read, buffer.read);
	std::swap(write, buffer.write);
}

void LogBuffer::Push(const LogMessage& message) {
	if (write + sizeof(message) > buffer.size()) {
		buffer.resize(buffer.size() * 2 + sizeof(message));
	}
	buffer[write] = message;
	write++;
}

LogMessage LogBuffer::Pop() {
	LogMessage message = buffer[read];
	read++;
	return message;
}

LogBuffer2::LogBuffer2()
	: buffer(4 * 1024), read(0), write(0) {
}

char* LogBuffer2::Begin() {
	return static_cast<char*>(&buffer[read]);
}

bool LogBuffer2::Empty() {
	return read == write;
}

size_t LogBuffer2::ReadableSize() {
	return write - read;
}

size_t LogBuffer2::WriteableSize() {
	return buffer.size() - write;
}

void LogBuffer2::Reset() {
	read = write = 0;
}

void LogBuffer2::Swap(LogBuffer2& buffer) {
	this->buffer.swap(buffer.buffer);
	std::swap(read, buffer.read);
	std::swap(write, buffer.write);
}

void LogBuffer2::Push(const std::string& message) {
	if (write + message.length() > buffer.size()) {
		buffer.resize(buffer.size() * 2 + message.length());
	}
	std::copy(message.begin(), message.end(), &buffer[write]);
	write += message.length();
}

void LogBuffer2::Pop(size_t length) {
	read += length;
}