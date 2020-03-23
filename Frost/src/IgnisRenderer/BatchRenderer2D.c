#include "BatchRenderer2D.h"

#include "IgnisRenderer.h"

#define BATCHRENDERER2D_MAX_QUADS			32
#define BATCHRENDERER2D_VERTEX_SIZE			(3 + 2 + 1)
#define BATCHRENDERER2D_VERTICES_PER_QUAD	4
#define BATCHRENDERER2D_INDICES_PER_QUAD	6

#define BATCHRENDERER2D_INDEX_COUNT		(BATCHRENDERER2D_MAX_QUADS * BATCHRENDERER2D_INDICES_PER_QUAD)
#define BATCHRENDERER2D_VERTEX_COUNT	(BATCHRENDERER2D_MAX_QUADS * BATCHRENDERER2D_VERTICES_PER_QUAD)
#define BATCHRENDERER2D_BUFFER_SIZE		(BATCHRENDERER2D_VERTEX_COUNT * BATCHRENDERER2D_VERTEX_SIZE)

#define BATCHRENDERER2D_TEXTURES	8

typedef struct
{
	ignis_vertex_array vao;
	ignis_shader shader;

	float* vertices;
	size_t vertex_index;

	size_t quad_count;

	GLuint* texture_slots;
	size_t texture_slot_index;

	GLint uniform_location_view_proj;
} _BatchRenderer2DStorage;

static _BatchRenderer2DStorage _render_data;

void BatchRenderer2DInit(const char* vert, const char* frag)
{
	ignisGenerateVertexArray(&_render_data.vao);

	ignis_buffer_element layout[] =
	{
		{GL_FLOAT, 3, GL_FALSE},	/* position */
		{GL_FLOAT, 2, GL_FALSE},	/* Texture coords*/
		{GL_FLOAT, 1, GL_FALSE}		/* Texture index */
	};

	ignisAddArrayBufferLayout(&_render_data.vao, BATCHRENDERER2D_BUFFER_SIZE * sizeof(float), NULL, GL_DYNAMIC_DRAW, layout, 3);

	GLuint indices[BATCHRENDERER2D_INDEX_COUNT];
	GenerateIndices(indices, BATCHRENDERER2D_INDEX_COUNT, BATCHRENDERER2D_INDICES_PER_QUAD);

	ignisLoadElementBuffer(&_render_data.vao, indices, BATCHRENDERER2D_INDEX_COUNT, GL_STATIC_DRAW);

	ignisShadervf(&_render_data.shader, vert, frag);

	ignisUseShader(&_render_data.shader);
	int samplers[BATCHRENDERER2D_TEXTURES];
	for (int i = 0; i < BATCHRENDERER2D_TEXTURES; i++)
	{
		samplers[i] = i;
	}
	ignisSetUniform1iv(&_render_data.shader, "u_Textures", BATCHRENDERER2D_TEXTURES, samplers);

	_render_data.uniform_location_view_proj = ignisGetUniformLocation(&_render_data.shader, "u_ViewProjection");

	_render_data.vertices = (float*)malloc(BATCHRENDERER2D_BUFFER_SIZE * sizeof(float));
	_render_data.vertex_index = 0;
	_render_data.quad_count = 0;

	_render_data.texture_slots = (GLuint*)malloc(BATCHRENDERER2D_TEXTURES * sizeof(GLuint));
	memset(_render_data.texture_slots, 0, BATCHRENDERER2D_TEXTURES * sizeof(GLuint));
	_render_data.texture_slot_index = 0;
}

void BatchRenderer2DDestroy()
{
	free(_render_data.vertices);
	free(_render_data.texture_slots);

	ignisDeleteShader(&_render_data.shader);
	ignisDeleteVertexArray(&_render_data.vao);
}

void BatchRenderer2DStart(const float* mat_view_proj)
{
	ignisUseShader(&_render_data.shader);
	ignisSetUniformMat4l(_render_data.uniform_location_view_proj, mat_view_proj);
}

void BatchRenderer2DFlush()
{
	if (_render_data.vertex_index == 0)
		return;

	ignisBindVertexArray(&_render_data.vao);
	ignisBufferSubData(&_render_data.vao.array_buffers[0], 0, _render_data.vertex_index * sizeof(float), _render_data.vertices);

	ignisUseShader(&_render_data.shader);

	/* bind textures */
	for (size_t i = 0; i < _render_data.texture_slot_index; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, _render_data.texture_slots[i]);
	}

	glDrawElements(GL_TRIANGLES, BATCHRENDERER2D_INDICES_PER_QUAD * _render_data.quad_count, GL_UNSIGNED_INT, NULL);

	_render_data.vertex_index = 0;
	_render_data.quad_count = 0;

	/* textures */
	memset(_render_data.texture_slots, 0, BATCHRENDERER2D_TEXTURES * sizeof(GLuint));
	_render_data.texture_slot_index = 0;
}

void _BatchRenderer2DPushValue(float value)
{
	if (_render_data.vertex_index >= BATCHRENDERER2D_BUFFER_SIZE)
	{
		_ignisErrorCallback(IGNIS_WARN, "[BatchRenderer2D] Vertex overflow");
		return;
	}

	_render_data.vertices[_render_data.vertex_index++] = value;
}

void BatchRenderer2DRenderTexture(ignis_texture* texture, float x, float y, float w, float h)
{
	if (_render_data.quad_count >= BATCHRENDERER2D_MAX_QUADS || _render_data.texture_slot_index >= BATCHRENDERER2D_TEXTURES)
		BatchRenderer2DFlush();

	float texture_index = -1.0f;
	for (size_t i = 0; i < _render_data.texture_slot_index; i++)
	{
		if (_render_data.texture_slots[i] == texture->name)
		{
			texture_index = (float)i;
			break;
		}
	}

	if (texture_index < 0.0f)
	{
		texture_index = (float)_render_data.texture_slot_index;
		_render_data.texture_slots[_render_data.texture_slot_index++] = texture->name;
	}

	/* BOTTOM LEFT */
	_BatchRenderer2DPushValue(x);
	_BatchRenderer2DPushValue(y);
	_BatchRenderer2DPushValue(0.0f);

	_BatchRenderer2DPushValue(0.0f);
	_BatchRenderer2DPushValue(0.0f);

	_BatchRenderer2DPushValue(texture_index);

	/* BOTTOM RIGHT */
	_BatchRenderer2DPushValue(x + w);
	_BatchRenderer2DPushValue(y);
	_BatchRenderer2DPushValue(0.0f);

	_BatchRenderer2DPushValue(1.0f);
	_BatchRenderer2DPushValue(0.0f);

	_BatchRenderer2DPushValue(texture_index);

	/* TOP RIGHT */
	_BatchRenderer2DPushValue(x + w);
	_BatchRenderer2DPushValue(y + h);
	_BatchRenderer2DPushValue(0.0f);

	_BatchRenderer2DPushValue(1.0f);
	_BatchRenderer2DPushValue(1.0f);

	_BatchRenderer2DPushValue(texture_index);

	/* TOP LEFT */
	_BatchRenderer2DPushValue(x);
	_BatchRenderer2DPushValue(y + h);
	_BatchRenderer2DPushValue(0.0f);

	_BatchRenderer2DPushValue(0.0f);
	_BatchRenderer2DPushValue(1.0f);

	_BatchRenderer2DPushValue(texture_index);

	_render_data.quad_count++;
}