#include "Primitives2D.h"

#include <math.h>

#define PRIMITIVES2D_MAX_LINES		(2 * 1024)
#define PRIMITIVES2D_VERTEX_SIZE	(2 + 4)

#define PRIMITIVES2D_BUFFER_SIZE	(PRIMITIVES2D_VERTEX_SIZE * PRIMITIVES2D_MAX_LINES)

typedef struct
{
	ignis_vertex_array vao;
	ignis_shader shader;

	float* vertices;
	GLsizei vertex_count;
} _Primitives2DStorage;

static _Primitives2DStorage _render_data;

void Primitives2DInit(const char* vert, const char* frag)
{
	ignisGenerateVertexArray(&_render_data.vao);

	ignis_buffer_element layout[] = 
	{
		{GL_FLOAT, 2, GL_FALSE},
		{GL_FLOAT, 4, GL_FALSE}
	};

	ignisAddArrayBufferLayout(&_render_data.vao, PRIMITIVES2D_BUFFER_SIZE * sizeof(float), NULL, GL_DYNAMIC_DRAW, layout, 2);

	_render_data.vertices = (float*)malloc(PRIMITIVES2D_BUFFER_SIZE * sizeof(float));
	_render_data.vertex_count = 0;

	ignisShadervf(&_render_data.shader, vert, frag);
}

void Primitives2DDestroy()
{
	ignisDeleteShader(&_render_data.shader);

	ignisDeleteVertexArray(&_render_data.vao);

	free(_render_data.vertices);
}

void Primitives2DStart(const float* mat_view_proj)
{
	ignisUseShader(&_render_data.shader);
	ignisSetUniformMat4(&_render_data.shader, "u_ViewProjection", mat_view_proj);
}

void Primitives2DFlush()
{
	if (_render_data.vertex_count == 0)
		return;

	ignisBindVertexArray(&_render_data.vao);
	ignisBufferSubData(&_render_data.vao.array_buffers[0], 0, _render_data.vertex_count * sizeof(float), _render_data.vertices);

	ignisUseShader(&_render_data.shader);

	glDrawArrays(GL_LINES, 0, _render_data.vertex_count / PRIMITIVES2D_VERTEX_SIZE);

	_render_data.vertex_count = 0;
}

void _Primitives2DVertex(float x, float y, const ignis_color_rgba color)
{
	if (_render_data.vertex_count >= PRIMITIVES2D_BUFFER_SIZE)
		Primitives2DFlush();

	_render_data.vertices[_render_data.vertex_count++] = x;
	_render_data.vertices[_render_data.vertex_count++] = y;

	_render_data.vertices[_render_data.vertex_count++] = color.r;
	_render_data.vertices[_render_data.vertex_count++] = color.g;
	_render_data.vertices[_render_data.vertex_count++] = color.b;
	_render_data.vertices[_render_data.vertex_count++] = color.a;
}

void Primitives2DRenderLine(float x1, float y1, float x2, float y2, const ignis_color_rgba color)
{
	_Primitives2DVertex(x1, y1, color);
	_Primitives2DVertex(x2, y2, color);
}

void Primitives2DRenderRect(float x, float y, float w, float h, const ignis_color_rgba color)
{
	float vertices[] =
	{
		x + 0, y + 0,
		x + w, y + 0,
		x + w, y + h,
		x + 0, y + h
	};

	Primitives2DRenderPolygon(vertices, 8, color);
}

void Primitives2DRenderPolygon(float* vertices, size_t count, const ignis_color_rgba color)
{
	if (!vertices || count < 2) return;

	float p1x = vertices[count - 2];
	float p1y = vertices[count - 1];

	size_t i = 0;
	while (i < count - 1)
	{
		float p2x = vertices[i++];
		float p2y = vertices[i++];

		_Primitives2DVertex(p1x, p1y, color);
		_Primitives2DVertex(p2x, p2y, color);

		p1x = p2x;
		p1y = p2y;
	}
}

void Primitives2DRenderCircle(float x, float y, float radius, const ignis_color_rgba color)
{
	const float PI = 3.14159265359f;

	const float k_segments = 36;
	const float k_increment = 2.0f * PI / k_segments;

	float sinInc = sinf(k_increment);
	float cosInc = cosf(k_increment);

	float r1x = 1.0f;
	float r1y = 0.0f;

	float v1x = x + radius * r1x;
	float v1y = y + radius * r1y;

	for (int i = 0; i < k_segments; ++i)
	{
		/* Perform rotation to avoid additional trigonometry. */
		float r2x = cosInc * r1x - sinInc * r1y;
		float r2y = sinInc * r1x + cosInc * r1y;

		float v2x = x + radius * r2x;
		float v2y = y + radius * r2y;

		_Primitives2DVertex(v1x, v1y, color);
		_Primitives2DVertex(v2x, v2y, color);

		r1x = r2x;
		r1y = r2y;
		v1x = v2x;
		v1y = v2y;
	}
}
