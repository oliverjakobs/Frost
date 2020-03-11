#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

// You can #define IGNIS_ASSERT(x) before the #include to avoid using assert.h.
#ifndef IGNIS_ASSERT
#include <assert.h>
#define IGNIS_ASSERT(x) assert(x)
#endif

#include "glad/glad.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

	typedef enum
	{
		IGNIS_WARN = 0,
		IGNIS_ERROR = 1,
		IGNIS_CRITICAL = 2
	} ignisErrorLevel;

	void ignisSetErrorCallback(void (*callback)(ignisErrorLevel, const char*));
	void _ignisErrorCallback(ignisErrorLevel level, const char* fmt, ...);

#ifdef __cplusplus
}
#endif