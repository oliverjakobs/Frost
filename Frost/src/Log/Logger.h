#pragma once

#include <spdlog/spdlog.h>

// Usage: 
//
//Logger::SetFormat("%^[%T] %n: %v%$");
//
//Logger::SetLevel(LogLevel::Trace);
//
//Logger::Info("Easy padding in numbers like {:08d}", 12);
//Logger::Trace("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
//Logger::Warn("Support for floats {:03.2f}", 1.23456);
//Logger::Error("Positional args are {1} {0}.", "too", "supported");

enum class LogLevel
{
	Trace = SPDLOG_LEVEL_TRACE,
	Debug = SPDLOG_LEVEL_DEBUG,
	Info = SPDLOG_LEVEL_INFO,
	Warn = SPDLOG_LEVEL_WARN,
	Error = SPDLOG_LEVEL_ERROR
};

struct Logger
{
	inline static void SetLevel(LogLevel lvl)
	{
		spdlog::set_level((spdlog::level::level_enum)lvl);
	}

	inline static void SetFormat(const std::string& pattern)
	{
		spdlog::set_pattern(pattern);
	}

	template<typename... Args>
	inline static void Info(const char* fmt, const Args&... args)
	{
		spdlog::info(fmt, args...);
	}

	template<typename... Args>
	inline static void Trace(const char* fmt, const Args&... args)
	{
		spdlog::trace(fmt, args...);
	}

	template<typename... Args>
	inline static void Warn(const char* fmt, const Args&... args)
	{
		spdlog::warn(fmt, args...);
	}

	template<typename... Args>
	inline static void Error(const char* fmt, const Args&... args)
	{
		spdlog::error(fmt, args...);
	}
};

#ifdef DEBUG
#define DEBUG_INFO(s, ...) Logger::Info(s, __VA_ARGS__)
#define DEBUG_TRACE(s, ...) Logger::Trace(s, __VA_ARGS__)
#define DEBUG_WARN(s, ...) Logger::Warn(s, __VA_ARGS__)
#define DEBUG_ERROR(s, ...) Logger::Error(s, __VA_ARGS__)
#else
#define DEBUG_INFO(s, ...)
#define DEBUG_TRACE(s, ...)
#define DEBUG_WARN(s, ...)
#define DEBUG_ERROR(s, ...)
#endif // DEBUG
