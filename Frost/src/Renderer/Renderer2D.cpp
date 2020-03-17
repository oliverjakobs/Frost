#include "Renderer2D.hpp"

#include <glm/gtc/matrix_transform.hpp>

struct Renderer2DStorage
{
	ignis_vertex_array* VertexArray;
	ignis_shader* Shader;

	GLint UniformLocationViewProjection;
	GLint UniformLocationColor;
	GLint UniformLocationModel;
	GLint UniformLocationSrc;
};

static Renderer2DStorage* s_renderData;

void Renderer2D::Init(ignis_shader* shader)
{
	s_renderData = new Renderer2DStorage();

	s_renderData->VertexArray = ignisGenerateVertexArray();

	float vertices[] =
	{
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	ignis_buffer_element layout[] =
	{
		{GL_FLOAT, 3, 0},
		{GL_FLOAT, 2, 0}
	};

	ignisAddArrayBufferLayout(s_renderData->VertexArray, sizeof(vertices), vertices, GL_STATIC_DRAW, layout, 2);

	GLuint indices[] = { 0, 1, 2, 2, 3, 0 };
	ignisLoadElementBuffer(s_renderData->VertexArray, indices, 6, GL_STATIC_DRAW);

	s_renderData->Shader = shader;
	s_renderData->UniformLocationViewProjection = ignisGetUniformLocation(s_renderData->Shader, "u_ViewProjection");
	s_renderData->UniformLocationColor = ignisGetUniformLocation(s_renderData->Shader, "u_Color");
	s_renderData->UniformLocationModel = ignisGetUniformLocation(s_renderData->Shader, "u_Model");
	s_renderData->UniformLocationSrc = ignisGetUniformLocation(s_renderData->Shader, "u_Src");

	ignisUseShader(s_renderData->Shader);
	ignisSetUniform1i(s_renderData->Shader, "u_Texture", 0);
}

void Renderer2D::Destroy()
{
	ignisDeleteShader(s_renderData->Shader);
	delete s_renderData;
}

void Renderer2D::Start(const glm::mat4& viewProjection)
{
	ignisUseShader(s_renderData->Shader);
	ignisSetUniformMat4l(s_renderData->UniformLocationViewProjection, &viewProjection[0][0]);
}

void Renderer2D::Flush()
{
}

void Renderer2D::RenderTexture(ignis_texture* texture, const glm::mat4& model, const glm::mat4& src, const color& color)
{
	ignisSetUniform4fl(s_renderData->UniformLocationColor, &color[0]);
	ignisSetUniformMat4l(s_renderData->UniformLocationModel, &model[0][0]);
	ignisSetUniformMat4l(s_renderData->UniformLocationSrc, &src[0][0]);

	ignisBindTexture(texture, 0);

	ignisBindVertexArray(s_renderData->VertexArray);
	glDrawElements(GL_TRIANGLES, s_renderData->VertexArray->element_buffer.count, GL_UNSIGNED_INT, nullptr);
}
