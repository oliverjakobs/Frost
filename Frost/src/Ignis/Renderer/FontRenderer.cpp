#include "FontRenderer.h"

namespace ignis
{
	struct FontRendererStorage
	{
		std::shared_ptr<VertexArray> VertexArray;
		std::shared_ptr<Shader> Shader;

		size_t BufferSize;
	};

	static FontRendererStorage* s_renderData;

	void FontRenderer::Init(const std::shared_ptr<Shader>& shader)
	{
		s_renderData = new FontRendererStorage();

		s_renderData->BufferSize = sizeof(float) * 4 * 4;

		s_renderData->VertexArray = std::make_shared<VertexArray>();
		s_renderData->VertexArray->AddArrayBuffer(std::make_shared<ArrayBuffer>(s_renderData->BufferSize, nullptr, GL_DYNAMIC_DRAW), { {GL_FLOAT, 4} });
		s_renderData->VertexArray->LoadElementBuffer({ 0,1,2,2,3,0 }, GL_STATIC_DRAW);

		s_renderData->Shader = shader;
	}

	void FontRenderer::Destroy()
	{
		delete s_renderData;
	}

	void FontRenderer::RenderText(Font& font, const std::string& text, float x, float y, const glm::mat4& proj, const ignis::color& color)
	{
		s_renderData->Shader->Use();
		s_renderData->Shader->SetUniformMat4("u_Projection", proj);
		s_renderData->Shader->SetUniform4f("u_Color", color);

		font.Bind();
		s_renderData->VertexArray->Bind();

		for (auto& c : text)
		{
			auto vertices = font.LoadCharQuad(c, &x, &y);

			if (!vertices.empty())
			{
				// Update content of VBO memory
				s_renderData->VertexArray->GetArrayBuffer(0)->BufferSubData(0, s_renderData->BufferSize, vertices.data());

				// Render quad
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
		}
	}
}
