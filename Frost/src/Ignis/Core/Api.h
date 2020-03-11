#pragma once

#include "Ignis/glad/glad.h"

// You can #define IGNIS_ASSERT(x) before the #include to avoid using assert.h.
#ifndef IGNIS_ASSERT
#include <assert.h>
#define IGNIS_ASSERT(x) assert(x)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdarg.h>

	enum class ignisErrorLevel
	{
		Warn = 0,
		Error = 1,
		Critical = 2
	};

	void ignisSetErrorCallback(void (*callback)(ignisErrorLevel, const char*));
	void _ignisErrorCallback(ignisErrorLevel level, const char* fmt, ...);

#ifdef __cplusplus
}
#endif