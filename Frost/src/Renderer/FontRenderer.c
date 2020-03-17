#include "FontRenderer.h"

#define FONTRENDERER_MAX_QUADS			32
#define FONTRENDERER_VERTEX_SIZE		4
#define FONTRENDERER_VERTICES_PER_QUAD	4
#define FONTRENDERER_INDICES_PER_QUAD	6

#define FONTRENDERER_INDEX_COUNT	(FONTRENDERER_MAX_QUADS * FONTRENDERER_INDICES_PER_QUAD)
#define FONTRENDERER_VERTEX_COUNT	(FONTRENDERER_MAX_QUADS * FONTRENDERER_VERTICES_PER_QUAD)
#define FONTRENDERER_BUFFER_SIZE	(FONTRENDERER_VERTEX_COUNT * FONTRENDERER_VERTEX_SIZE)

typedef struct 
{
	ignis_vertex_array* VertexArray;
	ignis_shader* Shader;

	GLint UniformLocationProjection;
	GLint UniformLocationColor;
} _FontRendererStorage;

static _FontRendererStorage* _render_data;

void FontRendererInit(ignis_shader* shader)
{
	_render_data = (_FontRendererStorage*)malloc(sizeof(_FontRendererStorage));

	_render_data->VertexArray = ignisGenerateVertexArray();

	ignis_buffer_element layout[] = { {GL_FLOAT, 4, 0} };
	ignisAddArrayBufferLayout(_render_data->VertexArray, FONTRENDERER_BUFFER_SIZE * sizeof(float), NULL, GL_DYNAMIC_DRAW, layout, 1);

	GLuint indices[FONTRENDERER_INDEX_COUNT];
	GLuint offset = 0;
	for (size_t i = 0; i < FONTRENDERER_INDEX_COUNT - FONTRENDERER_INDICES_PER_QUAD; i += FONTRENDERER_INDICES_PER_QUAD)
	{
		indices[i + 0] = offset + 0;
		indices[i + 1] = offset + 1;
		indices[i + 2] = offset + 2;

		indices[i + 3] = offset + 2;
		indices[i + 4] = offset + 3;
		indices[i + 5] = offset + 0;

		offset += 4;
	}

	ignisLoadElementBuffer(_render_data->VertexArray, indices, FONTRENDERER_INDEX_COUNT, GL_STATIC_DRAW);

	_render_data->Shader = shader;

	_render_data->UniformLocationProjection = ignisGetUniformLocation(_render_data->Shader, "u_Projection");
	_render_data->UniformLocationColor = ignisGetUniformLocation(_render_data->Shader, "u_Color");
}

void FontRendererDestroy()
{
	ignisDeleteShader(_render_data->Shader);
	ignisDeleteVertexArray(_render_data->VertexArray);

	free(_render_data);
}

static void _FontRendererFlush(float* vertices)
{
	// Update content of VBO memory
	ignisBufferSubData(&_render_data->VertexArray->array_buffers[0], 0, FONTRENDERER_BUFFER_SIZE * sizeof(float), vertices);

	// Render quad
	glDrawElements(GL_TRIANGLES, FONTRENDERER_INDEX_COUNT, GL_UNSIGNED_INT, 0);
}

void FontRendererRenderText(ignis_font* font, const char* text, float x, float y, const float* mat_proj, const ignis_color_rgba color)
{
	if (!(_render_data && font)) return;

	ignisUseShader(_render_data->Shader);
	ignisSetUniformMat4l(_render_data->UniformLocationProjection, mat_proj);
	ignisSetUniform4fl(_render_data->UniformLocationColor, &color.r);

	ignisBindFont(font);
	ignisBindVertexArray(_render_data->VertexArray);

	size_t offset = 0;

	float vertices[FONTRENDERER_BUFFER_SIZE];
	for (size_t i = 0; i < strlen(text); i++)
	{
		if (offset + FONTRENDERER_BUFFER_SIZE >= FONTRENDERER_BUFFER_SIZE)
		{
			_FontRendererFlush(vertices);
			offset = 0;
		}

		if (!ignisLoadCharQuad(font, text[i], &x, &y, vertices, offset))
			_ignisErrorCallback(IGNIS_WARN, "[FontRenderer] Failed to load quad for %c", text[i]);

		offset += FONTRENDERER_VERTICES_PER_QUAD * FONTRENDERER_VERTEX_SIZE;
	}

	_FontRendererFlush(vertices);
}
