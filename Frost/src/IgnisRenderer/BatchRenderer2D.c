#include "BatchRenderer2D.h"

#define BATCHRENDERER2D_MAX_QUADS			32
#define BATCHRENDERER2D_VERTEX_SIZE			(3 + 2 + 1)
#define BATCHRENDERER2D_VERTICES_PER_QUAD	4
#define BATCHRENDERER2D_INDICES_PER_QUAD	6

#define BATCHRENDERER2D_INDEX_COUNT		(BATCHRENDERER2D_MAX_QUADS * BATCHRENDERER2D_INDICES_PER_QUAD)
#define BATCHRENDERER2D_VERTEX_COUNT	(BATCHRENDERER2D_MAX_QUADS * BATCHRENDERER2D_VERTICES_PER_QUAD)
#define BATCHRENDERER2D_BUFFER_SIZE		(BATCHRENDERER2D_VERTEX_COUNT * BATCHRENDERER2D_VERTEX_SIZE)

typedef struct
{
	ignis_vertex_array vao;
	ignis_shader shader;

	float* vertices;
	size_t vertex_index;

	size_t quad_count;

	GLint uniform_location_view_proj;
} _BatchRenderer2DStorage;

static _BatchRenderer2DStorage _batch_render_data;

void BatchRenderer2DInit(const char* vert, const char* frag)
{
	ignisGenerateVertexArray(&_batch_render_data.vao);

	ignis_buffer_element layout[] =
	{
		{GL_FLOAT, 3, GL_FALSE},	/* position */
		{GL_FLOAT, 2, GL_FALSE},	/* Texture coords*/
		{GL_FLOAT, 1, GL_FALSE}		/* Texture index */
	};

	ignisAddArrayBufferLayout(&_batch_render_data.vao, BATCHRENDERER2D_BUFFER_SIZE * sizeof(float), NULL, GL_DYNAMIC_DRAW, layout, 2);

	_batch_render_data.vertices = (float*)malloc(BATCHRENDERER2D_VERTEX_COUNT * sizeof(float));
	_batch_render_data.vertex_index = 0;
	_batch_render_data.quad_count = 0;

	GLuint indices[BATCHRENDERER2D_INDEX_COUNT];
	GLuint offset = 0;
	for (size_t i = 0; i < BATCHRENDERER2D_INDEX_COUNT - BATCHRENDERER2D_INDICES_PER_QUAD; i += BATCHRENDERER2D_INDICES_PER_QUAD)
	{
		indices[i + 0] = offset + 0;
		indices[i + 1] = offset + 1;
		indices[i + 2] = offset + 2;

		indices[i + 3] = offset + 2;
		indices[i + 4] = offset + 3;
		indices[i + 5] = offset + 0;

		offset += 4;
	}

	// ignisLoadElementBuffer(&_render_data.vao, indices, BATCHRENDERER2D_INDEX_COUNT, GL_STATIC_DRAW);

	ignisShadervf(&_batch_render_data.shader, vert, frag);

	_batch_render_data.uniform_location_view_proj = ignisGetUniformLocation(&_batch_render_data.shader, "u_ViewProjection");
}

void BatchRenderer2DDestroy()
{
	free(_batch_render_data.vertices);

	ignisDeleteShader(&_batch_render_data.shader);
	ignisDeleteVertexArray(&_batch_render_data.vao);
}

void BatchRenderer2DStart(const float* mat_view_proj)
{
	ignisUseShader(&_batch_render_data.shader);
	ignisSetUniformMat4l(_batch_render_data.uniform_location_view_proj, mat_view_proj);
}

void BatchRenderer2DFlush()
{
	if (_batch_render_data.vertex_index == 0)
		return;

	ignisBindVertexArray(&_batch_render_data.vao);
	ignisBufferSubData(&_batch_render_data.vao.array_buffers[0], 0, _batch_render_data.vertex_index * sizeof(float), _batch_render_data.vertices);

	ignisUseShader(&_batch_render_data.shader);

	glDrawArrays(GL_LINES, 0, _batch_render_data.vertex_index / BATCHRENDERER2D_VERTEX_SIZE);

	_batch_render_data.vertex_index = 0;
}

void _BatchRenderer2DPushVertex(float x, float y, float z, float s, float t, float index)
{
	if (_batch_render_data.vertex_index >= BATCHRENDERER2D_BUFFER_SIZE)
		BatchRenderer2DFlush();

	_batch_render_data.vertices[_batch_render_data.vertex_index++] = x;
	_batch_render_data.vertices[_batch_render_data.vertex_index++] = y;
	_batch_render_data.vertices[_batch_render_data.vertex_index++] = z;

	_batch_render_data.vertices[_batch_render_data.vertex_index++] = s;
	_batch_render_data.vertices[_batch_render_data.vertex_index++] = t;

	_batch_render_data.vertices[_batch_render_data.vertex_index++] = index;
}

void BatchRenderer2DRenderTexture(ignis_texture* texture, float x, float y, float w, float h)
{	
	_BatchRenderer2DPushVertex(x, y, 0.0f, 0.0f, 0.0f, 0.0f);
	_BatchRenderer2DPushVertex(x + w, y, 0.0f, 1.0f, 0.0f, 0.0f);
	_BatchRenderer2DPushVertex(x + w, y + h, 0.0f, 1.0f, 1.0f, 0.0f);
	_BatchRenderer2DPushVertex(x, y + h, 0.0f, 0.0f, 1.0f, 0.0f);

	_batch_render_data.quad_count++;
}