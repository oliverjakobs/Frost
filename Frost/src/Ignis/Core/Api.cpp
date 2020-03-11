#include "Api.h"

#include <stdio.h>
#include <stdlib.h>

static void (*s_ignisErrorCallback)(ignisErrorLevel level, const char* fmt);

void ignisSetErrorCallback(void (*callback)(ignisErrorLevel, const char*))
{
	s_ignisErrorCallback = callback;
}

void _ignisErrorCallback(ignisErrorLevel level, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	size_t buffer_size = vsnprintf(NULL, 0, fmt, args);
	char* buffer = (char*)malloc(buffer_size + 1);
	vsnprintf(buffer, buffer_size + 1, fmt, args);
	va_end(args);

	s_ignisErrorCallback(level, buffer);

	free(buffer);
}
