#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

struct Renderer2DStorage
{
	ignis_vertex_array* VertexArray;
	ignis_shader* Shader;
};

static Renderer2DStorage* s_renderData;

void Renderer2D::Init(ignis_shader* shader)
{
	s_renderData = new Renderer2DStorage();

	s_renderData->VertexArray = ignisGenerateVertexArray();

	float vertices[5 * 4] =
	{
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	ignis_buffer_element layout[2] =
	{
		{GL_FLOAT, 3, 0},
		{GL_FLOAT, 2, 0}
	};

	ignisAddArrayBufferLayout(s_renderData->VertexArray, sizeof(vertices), vertices, GL_STATIC_DRAW, layout, 2);

	GLuint indices[6] = { 0, 1, 2, 2, 3, 0 };
	ignisLoadElementBuffer(s_renderData->VertexArray, indices, 6, GL_STATIC_DRAW);

	s_renderData->Shader = shader;

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
	ignisSetUniformMat4(s_renderData->Shader, "u_ViewProjection", &viewProjection[0][0]);
}

void Renderer2D::Flush()
{
}

void Renderer2D::RenderTexture(ignis_texture* texture, const glm::mat4& model, const glm::mat4& src, const color& color)
{
	ignisSetUniform4f(s_renderData->Shader, "u_Color", &color[0]);
	ignisSetUniformMat4(s_renderData->Shader, "u_Model", &model[0][0]);
	ignisSetUniformMat4(s_renderData->Shader, "u_Src", &src[0][0]);

	ignisBindTexture(texture, 0);

	ignisBindVertexArray(s_renderData->VertexArray);
	glDrawElements(GL_TRIANGLES, s_renderData->VertexArray->element_buffer->count, GL_UNSIGNED_INT, nullptr);
}
