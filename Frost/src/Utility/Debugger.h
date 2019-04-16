#pragma once

#include "Log/Logger.h"

#ifdef _DEBUG
#define DEBUG_INFO(s, ...) Logger::Info(s, __VA_ARGS__)
#define DEBUG_TRACE(s, ...) Logger::Trace(s, __VA_ARGS__)
#define DEBUG_WARN(s, ...) Logger::Warn(s, __VA_ARGS__)
#define DEBUG_ERROR(s, ...) Logger::Error(s, __VA_ARGS__)

#define DEBUG_ASSERT(x, ...) { if(!(x)) { DEBUG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

#define DEBUG_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
#define DEBUG_INFO(s, ...)
#define DEBUG_TRACE(s, ...)
#define DEBUG_WARN(s, ...)
#define DEBUG_ERROR(s, ...)

#define DEBUG_ASSERT(x, ...)

#define DEBUG_NEW new
#endif // DEBUG
