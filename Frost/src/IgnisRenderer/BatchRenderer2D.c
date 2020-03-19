#include "BatchRenderer2D.h"

typedef struct
{
	ignis_vertex_array vao;
	ignis_shader shader;

	GLint uniform_location_view_proj;
} _BatchRenderer2DStorage;

static _BatchRenderer2DStorage* _render_data;

void BatchRenderer2DInit(const char* vert, const char* frag)
{
	_render_data = (_BatchRenderer2DStorage*)malloc(sizeof(_BatchRenderer2DStorage));

	ignisGenerateVertexArray(&_render_data->vao);

	ignis_buffer_element layout[] =
	{
		{GL_FLOAT, 3, GL_FALSE},
		{GL_FLOAT, 2, GL_FALSE}
	};

	ignisAddArrayBufferLayout(&_render_data->vao, sizeof(float), NULL, GL_DYNAMIC_DRAW, layout, 2);

	GLuint indices[] = { 0, 1, 2, 2, 3, 0 };
	ignisLoadElementBuffer(&_render_data->vao, indices, 6, GL_STATIC_DRAW);

	ignisShadervf(&_render_data->shader, vert, frag);

	_render_data->uniform_location_view_proj = ignisGetUniformLocation(&_render_data->shader, "u_ViewProjection");
}

void BatchRenderer2DDestroy()
{
	ignisDeleteShader(&_render_data->shader);
	ignisDeleteVertexArray(&_render_data->vao);

	free(_render_data);
}

void BatchRenderer2DStart(const float* mat_view_proj)
{
	ignisUseShader(&_render_data->shader);
	ignisSetUniformMat4l(_render_data->uniform_location_view_proj, mat_view_proj);
}

void BatchRenderer2DFlush()
{
}

void BatchRenderer2DRenderTexture(ignis_texture* texture, const float* mat_model, const float* mat_src)
{
}

void BatchRenderer2DRenderTextureColor(ignis_texture* texture, const float* mat_model, const float* mat_src, const ignis_color_rgba color)
{
}
