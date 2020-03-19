#include "Renderer2D.h"

typedef struct
{
	ignis_vertex_array vao;
	ignis_shader shader;

	GLint uniform_location_view_proj;
	GLint uniform_location_color;
	GLint uniform_location_model;
	GLint uniform_location_src;
} _Renderer2DStorage;

static _Renderer2DStorage* _render_data;

void Renderer2DInit(const char* vert, const char* frag)
{
	_render_data = (_Renderer2DStorage*)malloc(sizeof(_Renderer2DStorage));

	ignisGenerateVertexArray(&_render_data->vao);

	float vertices[] =
	{
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	ignis_buffer_element layout[] =
	{
		{GL_FLOAT, 3, GL_FALSE},
		{GL_FLOAT, 2, GL_FALSE}
	};

	ignisAddArrayBufferLayout(&_render_data->vao, sizeof(vertices), vertices, GL_STATIC_DRAW, layout, 2);

	GLuint indices[] = { 0, 1, 2, 2, 3, 0 };
	ignisLoadElementBuffer(&_render_data->vao, indices, 6, GL_STATIC_DRAW);

	ignisShadervf(&_render_data->shader, vert, frag);

	_render_data->uniform_location_view_proj = ignisGetUniformLocation(&_render_data->shader, "u_ViewProjection");
	_render_data->uniform_location_color = ignisGetUniformLocation(&_render_data->shader, "u_Color");
	_render_data->uniform_location_model = ignisGetUniformLocation(&_render_data->shader, "u_Model");
	_render_data->uniform_location_src = ignisGetUniformLocation(&_render_data->shader, "u_Src");

	ignisUseShader(&_render_data->shader);
	ignisSetUniform1i(&_render_data->shader, "u_Texture", 0);
}

void Renderer2DDestroy()
{
	ignisDeleteShader(&_render_data->shader);
	ignisDeleteVertexArray(&_render_data->vao);

	free(_render_data);
}

void Renderer2DStart(const float* mat_view_proj)
{
	ignisUseShader(&_render_data->shader);
	ignisSetUniformMat4l(_render_data->uniform_location_view_proj, mat_view_proj);
}

void Renderer2DFlush()
{
}

void Renderer2DRenderTexture(ignis_texture* texture, const float* mat_model, const float* mat_src)
{
	Renderer2DRenderTextureColor(texture, mat_model, mat_src, IGNIS_WHITE);
}

void Renderer2DRenderTextureColor(ignis_texture* texture, const float* mat_model, const float* mat_src, const ignis_color_rgba color)
{
	ignisSetUniform4fl(_render_data->uniform_location_color, &color.r);
	ignisSetUniformMat4l(_render_data->uniform_location_model, mat_model);
	ignisSetUniformMat4l(_render_data->uniform_location_src, mat_src);

	ignisBindTexture(texture, 0);

	ignisBindVertexArray(&_render_data->vao);
	glDrawElements(GL_TRIANGLES, _render_data->vao.element_buffer.count, GL_UNSIGNED_INT, NULL);
}