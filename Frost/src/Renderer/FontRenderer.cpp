#include "FontRenderer.h"

#include <map>

const size_t FONT_BUFFER_SIZE = sizeof(float) * 4 * 4;

struct FontRendererStorage
{
	ignis_vertex_array* VertexArray;
	ignis_shader* Shader;

	std::map<std::string, ignis_font*> Fonts;

	GLint UniformLocationProjection;
	GLint UniformLocationColor;
};

static FontRendererStorage* s_renderData;

void FontRenderer::Init(ignis_shader* shader)
{
	s_renderData = new FontRendererStorage();

	s_renderData->VertexArray = ignisGenerateVertexArray();

	ignis_buffer_element layout[] =
	{
			{GL_FLOAT, 4, 0}
	};
	ignisAddArrayBufferLayout(s_renderData->VertexArray, FONT_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW, layout, 1);

	GLuint indices[] = { 0,1,2,2,3,0 };

	ignisLoadElementBuffer(s_renderData->VertexArray, indices, 6, GL_STATIC_DRAW);

	s_renderData->Shader = shader;

	s_renderData->UniformLocationProjection = ignisGetUniformLocation(s_renderData->Shader, "u_Projection");
	s_renderData->UniformLocationColor = ignisGetUniformLocation(s_renderData->Shader, "u_Color");
}

void FontRenderer::Destroy()
{
	ignisDeleteShader(s_renderData->Shader);

	for (auto& font : s_renderData->Fonts)
		ignisDeleteFont(font.second);

	delete s_renderData;
}

void FontRenderer::AddFont(const std::string& name, ignis_font* font)
{
	if (s_renderData->Fonts.find(name) != s_renderData->Fonts.end())
	{
		_ignisErrorCallback(IGNIS_WARN, "[FONT] Name %s already in use", name.c_str());
		return;
	}

	s_renderData->Fonts.insert({ name, font });
}

ignis_font* FontRenderer::GetFont(const std::string& name)
{
	auto font = s_renderData->Fonts.find(name);

	if (font != s_renderData->Fonts.end())
		return font->second;

	_ignisErrorCallback(IGNIS_WARN, "[FONT] Font %s not found", name.c_str());
	return nullptr;
}

void FontRenderer::RenderText(const std::string& fontname, const std::string& text, float x, float y, const glm::mat4& proj, const color& color)
{
	auto font = GetFont(fontname);

	if (!font) return;

	ignisUseShader(s_renderData->Shader);
	ignisSetUniformMat4l(s_renderData->UniformLocationProjection, &proj[0][0]);
	ignisSetUniform4fl(s_renderData->UniformLocationColor, &color[0]);

	ignisBindFont(font);
	ignisBindVertexArray(s_renderData->VertexArray);

	for (auto& c : text)
	{
		float vertices[16];
		if (ignisLoadFontCharQuad(font, c, &x, &y, vertices))
		{
			// Update content of VBO memory
			ignisBufferSubData(&s_renderData->VertexArray->array_buffers[0], 0, FONT_BUFFER_SIZE, vertices);

			// Render quad
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}
}
