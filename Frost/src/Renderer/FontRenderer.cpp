#include "FontRenderer.h"

#define MAX_QUADS			32
#define VERTEX_SIZE			4
#define VERTICES_PER_QUAD	4
#define INDICES_PER_QUAD	6

#define FONT_INDEX_COUNT	MAX_QUADS * INDICES_PER_QUAD
#define FONT_VERTEX_COUNT	MAX_QUADS * VERTICES_PER_QUAD
#define FONT_BUFFER_SIZE	FONT_VERTEX_COUNT * VERTEX_SIZE * sizeof(float)

struct FontRendererStorage
{
	ignis_vertex_array* VertexArray;
	ignis_shader* Shader;

	GLint UniformLocationProjection;
	GLint UniformLocationColor;
};

static FontRendererStorage* s_renderData;

void FontRenderer::Init(ignis_shader* shader)
{
	s_renderData = new FontRendererStorage();

	s_renderData->VertexArray = ignisGenerateVertexArray();

	ignis_buffer_element layout[] =
	{
		{GL_FLOAT, 4, 0}
	};
	ignisAddArrayBufferLayout(s_renderData->VertexArray, FONT_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW, layout, 1);

	GLuint indices[FONT_INDEX_COUNT];
	GLuint offset = 0;
	for (size_t i = 0; i < FONT_INDEX_COUNT - INDICES_PER_QUAD; i += INDICES_PER_QUAD)
	{ 
		indices[i + 0] = offset + 0;
		indices[i + 1] = offset + 1;
		indices[i + 2] = offset + 2;

		indices[i + 3] = offset + 2;
		indices[i + 4] = offset + 3;
		indices[i + 5] = offset + 0;

		offset += 4;
	}

	ignisLoadElementBuffer(s_renderData->VertexArray, indices, FONT_INDEX_COUNT, GL_STATIC_DRAW);

	s_renderData->Shader = shader;

	s_renderData->UniformLocationProjection = ignisGetUniformLocation(s_renderData->Shader, "u_Projection");
	s_renderData->UniformLocationColor = ignisGetUniformLocation(s_renderData->Shader, "u_Color");
}

void FontRenderer::Destroy()
{
	ignisDeleteShader(s_renderData->Shader);
	ignisDeleteVertexArray(s_renderData->VertexArray);

	delete s_renderData;
}

static void _flushTextData(float* vertices)
{
	// Update content of VBO memory
	ignisBufferSubData(&s_renderData->VertexArray->array_buffers[0], 0, FONT_BUFFER_SIZE, vertices);

	// Render quad
	glDrawElements(GL_TRIANGLES, FONT_INDEX_COUNT, GL_UNSIGNED_INT, 0);
}

void FontRenderer::RenderText(ignis_font* font, const char* text, float x, float y, const glm::mat4& proj, const color& color)
{
	if (!font) return;

	ignisUseShader(s_renderData->Shader);
	ignisSetUniformMat4l(s_renderData->UniformLocationProjection, &proj[0][0]);
	ignisSetUniform4fl(s_renderData->UniformLocationColor, &color[0]);

	ignisBindFont(font);
	ignisBindVertexArray(s_renderData->VertexArray);

	size_t offset = 0;

	float vertices[FONT_VERTEX_COUNT * VERTEX_SIZE];
	for (size_t i = 0; i < strlen(text); i++)
	{
		if (offset + VERTICES_PER_QUAD * VERTEX_SIZE >= FONT_VERTEX_COUNT * VERTEX_SIZE)
		{
			_flushTextData(vertices);
			offset = 0;
		}

		if (!ignisLoadCharQuad(font, text[i], &x, &y, vertices, offset))
			_ignisErrorCallback(IGNIS_WARN, "[FontRenderer] Failed to load quad for %c", text[i]);

		offset += VERTICES_PER_QUAD * VERTEX_SIZE;
	}
	
	_flushTextData(vertices);
}