#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

namespace ignis
{
	struct Renderer2DStorage
	{
		std::shared_ptr<VertexArray> VertexArray;
		std::shared_ptr<Shader> Shader;
	};

	static Renderer2DStorage* s_renderData;

	void Renderer2D::Init(const std::shared_ptr<Shader>& shader)
	{
		s_renderData = new Renderer2DStorage();

		s_renderData->VertexArray = std::make_shared<VertexArray>();

		float vertices[5 * 4] = 
		{
			0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f
		};

		s_renderData->VertexArray->AddArrayBuffer(std::make_shared<ArrayBuffer>(sizeof(vertices), vertices, GL_STATIC_DRAW),
			{
				{GL_FLOAT, 3},
				{GL_FLOAT, 2}
			});

		s_renderData->VertexArray->LoadElementBuffer({ 0, 1, 2, 2, 3, 0 }, GL_STATIC_DRAW);

		s_renderData->Shader = shader;
		s_renderData->Shader->Use();
		s_renderData->Shader->SetUniform1i("u_Texture", 0);
	}

	void Renderer2D::Destroy()
	{
		delete s_renderData;
	}

	void Renderer2D::Start(const glm::mat4& viewProjection)
	{
		s_renderData->Shader->Use();
		s_renderData->Shader->SetUniformMat4("u_ViewProjection", viewProjection);
	}

	void Renderer2D::Flush()
	{
	}

	void Renderer2D::RenderTexture(const std::shared_ptr<Texture>& texture, const glm::mat4& model, const glm::mat4& src, const color& color)
	{
		s_renderData->Shader->SetUniform4f("u_Color", color);
		s_renderData->Shader->SetUniformMat4("u_Model", model);
		s_renderData->Shader->SetUniformMat4("u_Src", src);

		texture->Bind();

		s_renderData->VertexArray->Bind();
		glDrawElements(GL_TRIANGLES, s_renderData->VertexArray->GetElementCount(), GL_UNSIGNED_INT, nullptr);
	}
}
