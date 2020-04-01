#include "FontRenderer.h"

#include "Renderer.h"

typedef struct 
{
	IgnisVertexArray vao;
	IgnisShader shader;

	IgnisFont* font;
	IgnisColorRGBA color;

	GLint uniform_location_proj;
	GLint uniform_location_color;
} _FontRendererStorage;

static _FontRendererStorage _render_data;

void FontRendererInit(const char* vert, const char* frag)
{
	ignisGenerateVertexArray(&_render_data.vao);

	IgnisBufferElement layout[] = { {GL_FLOAT, 4, GL_FALSE} };
	ignisAddArrayBufferLayout(&_render_data.vao, FONTRENDERER_BUFFER_SIZE * sizeof(float), NULL, GL_DYNAMIC_DRAW, layout, 1);

	GLuint indices[FONTRENDERER_INDEX_COUNT];
	GenerateIndices(indices, FONTRENDERER_INDEX_COUNT, FONTRENDERER_INDICES_PER_QUAD);

	ignisLoadElementBuffer(&_render_data.vao, indices, FONTRENDERER_INDEX_COUNT, GL_STATIC_DRAW);

	_render_data.font = NULL;
	_render_data.color = IGNIS_WHITE;

	ignisShadervf(&_render_data.shader, vert, frag);

	_render_data.uniform_location_proj = ignisGetUniformLocation(&_render_data.shader, "u_Projection");
	_render_data.uniform_location_color = ignisGetUniformLocation(&_render_data.shader, "u_Color");
}

void FontRendererDestroy()
{
	ignisDeleteShader(&_render_data.shader);

	ignisDeleteVertexArray(&_render_data.vao);
}

void FontRendererBindFont(IgnisFont* font)
{
	_render_data.font = font;
}

void FontRendererSetColor(IgnisColorRGBA color)
{
	_render_data.color = color;
}

static void _FontRendererFlush(float* vertices)
{
	/* Update content of VBO memory */
	ignisBufferSubData(&_render_data.vao.array_buffers[0], 0, FONTRENDERER_BUFFER_SIZE * sizeof(float), vertices);

	/* Render quad */
	glDrawElements(GL_TRIANGLES, FONTRENDERER_INDEX_COUNT, GL_UNSIGNED_INT, 0);
}

void FontRendererRenderText(float x, float y, const float* mat_proj, const char* text)
{
	if (!_render_data.font)
	{
		_ignisErrorCallback(IGNIS_WARN, "[FontRenderer] No font bound");
		return;
	}

	ignisUseShader(&_render_data.shader);
	ignisSetUniformMat4l(_render_data.uniform_location_proj, mat_proj);
	ignisSetUniform4fl(_render_data.uniform_location_color, &_render_data.color.r);

	ignisBindFont(_render_data.font);
	ignisBindVertexArray(&_render_data.vao);

	size_t offset = 0;

	float vertices[FONTRENDERER_BUFFER_SIZE];
	for (size_t i = 0; i < strlen(text); i++)
	{
		if (offset + FONTRENDERER_BUFFER_SIZE >= FONTRENDERER_BUFFER_SIZE)
		{
			_FontRendererFlush(vertices);
			offset = 0;
		}

		if (!ignisLoadCharQuad(_render_data.font, text[i], &x, &y, vertices, offset))
			_ignisErrorCallback(IGNIS_WARN, "[FontRenderer] Failed to load quad for %c", text[i]);

		offset += FONTRENDERER_VERTICES_PER_QUAD * FONTRENDERER_VERTEX_SIZE;
	}

	_FontRendererFlush(vertices);
}

void FontRendererRenderTextFormat(float x, float y, const float* mat_proj, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	size_t buffer_size = vsnprintf(NULL, 0, fmt, args);
	char* buffer = (char*)malloc(buffer_size + 1);
	vsnprintf(buffer, buffer_size + 1, fmt, args);
	va_end(args);

	FontRendererRenderText(x, y, mat_proj, buffer);

	free(buffer);
}
