#ifndef IGNIS_H
#define IGNIS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/* You can #define IGNIS_ASSERT(x) before the #include to avoid using assert.h */
#ifndef IGNIS_ASSERT
#include <assert.h>
#define IGNIS_ASSERT(x) assert(x)
#endif

#include "Texture.h"
#include "Shader.h"
#include "Buffer.h"
#include "Font.h"

int ignisInit(int debug);

typedef enum
{
	IGNIS_WARN = 0,
	IGNIS_ERROR = 1,
	IGNIS_CRITICAL = 2
} ignisErrorLevel;

void ignisSetErrorCallback(void (*callback)(ignisErrorLevel, const char*));
void _ignisErrorCallback(ignisErrorLevel level, const char* fmt, ...);

GLuint ignisGetOpenGLTypeSize(GLenum type);

#ifdef __cplusplus
}
#endif

#endif /* IGNIS_H */