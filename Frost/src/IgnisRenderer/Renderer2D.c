#include "Renderer2D.h"

typedef struct
{
	ignis_vertex_array* VertexArray;
	ignis_shader* Shader;

	GLint UniformLocationViewProjection;
	GLint UniformLocationColor;
	GLint UniformLocationModel;
	GLint UniformLocationSrc;
} _Renderer2DStorage;

static _Renderer2DStorage* _render_data;

void Renderer2DInit(ignis_shader* shader)
{
	_render_data = (_Renderer2DStorage*)malloc(sizeof(_Renderer2DStorage));

	_render_data->VertexArray = (ignis_vertex_array*)malloc(sizeof(ignis_vertex_array));
	ignisGenerateVertexArray(_render_data->VertexArray);

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

	ignisAddArrayBufferLayout(_render_data->VertexArray, sizeof(vertices), vertices, GL_STATIC_DRAW, layout, 2);

	GLuint indices[] = { 0, 1, 2, 2, 3, 0 };
	ignisLoadElementBuffer(_render_data->VertexArray, indices, 6, GL_STATIC_DRAW);

	_render_data->Shader = shader;
	_render_data->UniformLocationViewProjection = ignisGetUniformLocation(_render_data->Shader, "u_ViewProjection");
	_render_data->UniformLocationColor = ignisGetUniformLocation(_render_data->Shader, "u_Color");
	_render_data->UniformLocationModel = ignisGetUniformLocation(_render_data->Shader, "u_Model");
	_render_data->UniformLocationSrc = ignisGetUniformLocation(_render_data->Shader, "u_Src");

	ignisUseShader(_render_data->Shader);
	ignisSetUniform1i(_render_data->Shader, "u_Texture", 0);
}

void Renderer2DDestroy()
{
	ignisDeleteShader(_render_data->Shader);

	ignisDeleteVertexArray(_render_data->VertexArray);
	free(_render_data->VertexArray);

	free(_render_data);
}

void Renderer2DStart(const float* mat_view_proj)
{
	ignisUseShader(_render_data->Shader);
	ignisSetUniformMat4l(_render_data->UniformLocationViewProjection, mat_view_proj);
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
	ignisSetUniform4fl(_render_data->UniformLocationColor, &color.r);
	ignisSetUniformMat4l(_render_data->UniformLocationModel, mat_model);
	ignisSetUniformMat4l(_render_data->UniformLocationSrc, mat_src);

	ignisBindTexture(texture, 0);

	ignisBindVertexArray(_render_data->VertexArray);
	glDrawElements(GL_TRIANGLES, _render_data->VertexArray->element_buffer.count, GL_UNSIGNED_INT, NULL);
}
