#include "FontRenderer.h"

#include <map>

namespace ignis
{
	struct FontRendererStorage
	{
		std::shared_ptr<VertexArray> VertexArray;
		ignis_shader* Shader;

		std::map<std::string, std::shared_ptr<Font>> Fonts;

		size_t BufferSize;
	};

	static FontRendererStorage* s_renderData;

	void FontRenderer::Init(ignis_shader* shader)
	{
		s_renderData = new FontRendererStorage();

		s_renderData->BufferSize = sizeof(float) * 4 * 4;

		s_renderData->VertexArray = std::make_shared<VertexArray>();
		s_renderData->VertexArray->AddArrayBuffer(s_renderData->BufferSize, nullptr, GL_DYNAMIC_DRAW, { {GL_FLOAT, 4} });
		s_renderData->VertexArray->LoadElementBuffer({ 0,1,2,2,3,0 }, GL_STATIC_DRAW);

		s_renderData->Shader = shader;
	}

	void FontRenderer::Destroy()
	{
		ignisDeleteShader(s_renderData->Shader);
		delete s_renderData;
	}

	void FontRenderer::AddFont(const std::string& name, std::shared_ptr<Font> font)
	{
		if (s_renderData->Fonts.find(name) != s_renderData->Fonts.end())
		{
			_ignisErrorCallback(IGNIS_WARN, "[FONT] Name %s already in use", name.c_str());
			return;
		}

		s_renderData->Fonts.insert({ name, font });
	}

	std::shared_ptr<Font> FontRenderer::GetFont(const std::string& name)
	{
		auto font = s_renderData->Fonts.find(name);

		if (font != s_renderData->Fonts.end())
			return font->second;

		_ignisErrorCallback(IGNIS_WARN, "[FONT] Font %s not found", name.c_str());
		return nullptr;
	}

	void FontRenderer::RenderText(const std::string& fontname, const std::string& text, float x, float y, const glm::mat4& proj, const ignis::color& color)
	{
		auto font = GetFont(fontname);

		if (!font) return;

		ignisUseShader(s_renderData->Shader);
		ignisSetUniformMat4(s_renderData->Shader, "u_Projection", &proj[0][0]);
		ignisSetUniform4f(s_renderData->Shader, "u_Color", &color[0]);

		font->Bind();
		s_renderData->VertexArray->Bind();

		for (auto& c : text)
		{
			auto vertices = font->LoadCharQuad(c, &x, &y);

			if (!vertices.empty())
			{
				// Update content of VBO memory
				ignisArrayBufferSubData(s_renderData->VertexArray->GetArrayBuffer(0), 0, s_renderData->BufferSize, vertices.data());

				// Render quad
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
		}
	}
}
