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

/* Core */
#include "Core/Texture.h"
#include "Core/Shader.h"
#include "Core/Buffer.h"

/* Font */
#define IGNIS_FONT_VERTEX_SIZE			4
#define IGNIS_FONT_VERTICES_PER_QUAD	4
#define IGNIS_FONT_INDICES_PER_QUAD		4
#define IGNIS_FONT_MAX_QUADS			1024
#define IGNIS_FONT_MAX_VERTICES			IGNIS_FONT_MAX_QUADS * IGNIS_FONT_VERTICES_PER_QUAD
#define IGNIS_FONT_MAX_INDICES			IGNIS_FONT_MAX_QUADS * IGNIS_FONT_INDICES_PER_QUAD

#include "Font.h"

/* Vertex Array */
#define IGNIS_BUFFER_ARRAY_INITIAL_SIZE 4
#define IGNIS_BUFFER_ARRAY_GROWTH_FACTOR 2

#include "VertexArray.h"

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