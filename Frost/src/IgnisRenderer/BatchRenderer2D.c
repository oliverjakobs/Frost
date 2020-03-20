#include "BatchRenderer2D.h"

#define BATCHRENDERER2D_MAX_QUADS			32
#define BATCHRENDERER2D_VERTEX_SIZE			4
#define BATCHRENDERER2D_VERTICES_PER_QUAD	4
#define BATCHRENDERER2D_INDICES_PER_QUAD	6

#define BATCHRENDERER2D_INDEX_COUNT		(BATCHRENDERER2D_MAX_QUADS * BATCHRENDERER2D_INDICES_PER_QUAD)
#define BATCHRENDERER2D_VERTEX_COUNT	(BATCHRENDERER2D_MAX_QUADS * BATCHRENDERER2D_VERTICES_PER_QUAD)
#define BATCHRENDERER2D_BUFFER_SIZE		(BATCHRENDERER2D_VERTEX_COUNT * BATCHRENDERER2D_VERTEX_SIZE)

typedef struct
{
	ignis_vertex_array vao;
	ignis_shader shader;

	GLint uniform_location_view_proj;
} _BatchRenderer2DStorage;

static _BatchRenderer2DStorage _render_data;

void BatchRenderer2DInit(const char* vert, const char* frag)
{
	ignisGenerateVertexArray(&_render_data.vao);

	ignis_buffer_element layout[] =
	{
		{GL_FLOAT, 3, GL_FALSE},	/* position */
		{GL_FLOAT, 4, GL_FALSE},	/* color */
		{GL_FLOAT, 2, GL_FALSE},	/* Texture coords*/
		{GL_FLOAT, 1, GL_FALSE}		/* Texture index */
	};

	ignisAddArrayBufferLayout(&_render_data.vao, BATCHRENDERER2D_BUFFER_SIZE * sizeof(float), NULL, GL_DYNAMIC_DRAW, layout, 2);

	GLuint indices[] = { 0, 1, 2, 2, 3, 0 };
	ignisLoadElementBuffer(&_render_data.vao, indices, 6, GL_STATIC_DRAW);

	ignisShadervf(&_render_data.shader, vert, frag);

	_render_data.uniform_location_view_proj = ignisGetUniformLocation(&_render_data.shader, "u_ViewProjection");
}

void BatchRenderer2DDestroy()
{
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
}

void BatchRenderer2DRenderTexture(ignis_texture* texture, float x, float y, float width, float height)
{
}