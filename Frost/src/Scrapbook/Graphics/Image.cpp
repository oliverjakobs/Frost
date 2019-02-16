#include "Image.h"

#include "Renderer.h"

namespace sb
{
	std::vector<GLfloat> GetVertices(float w, float h, float fW, float fH)
	{
		return
		{
			0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			   w, 0.0f, 0.0f,   fW, 0.0f,
			   w,    h, 0.0f,   fW,   fH,
			0.0f,    h, 0.0f, 0.0f,   fH,
			// FLIP_HORIZONTAL
			0.0f, 0.0f, 0.0f,   fW, 0.0f,
			   w, 0.0f, 0.0f, 0.0f, 0.0f,
			   w,    h, 0.0f, 0.0f,   fH,
			0.0f,    h, 0.0f,   fW,   fH
		};
	}

	Image::Image(const std::string& src, float width, float height)
		: m_width(width), m_height(height)
	{
		auto vertices = GetVertices(width, height, 1.0f, 1.0f);

		m_flip = FLIP_NONE;
		
		m_vao.bind();

		m_vao.bindVertexBuffer();
		m_vao.setVertexBufferData(vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
		m_vao.setVertexAttribPointer(0, 3, 5, 0);
		m_vao.setVertexAttribPointer(1, 2, 5, 3);

		m_texture = unique_ptr<Texture>(new Texture(src.c_str(), 0));
	}

	Image::Image(const std::string & src, float width, float height, int rows, int columns)
		: m_width(width), m_height(height), m_rows(rows), m_columns(columns)
	{
		float fWidth = 1.0f / m_columns;
		float fHeight = 1.0f / m_rows;


		auto vertices = GetVertices(width, height, fWidth, fHeight);

		m_flip = FLIP_NONE;

		m_vao.bind();

		m_vao.bindVertexBuffer();
		m_vao.setVertexBufferData(vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
		m_vao.setVertexAttribPointer(0, 3, 5, 0);
		m_vao.setVertexAttribPointer(1, 2, 5, 3);

		m_texture = unique_ptr<Texture>(new Texture(src.c_str(), 0));
	}

	Image::~Image()
	{

	}

	void Image::setRenderFlip(RenderFlip flip)
	{
		m_flip = flip;
	}
	
	void Image::render(float x, float y, const glm::mat4& view, const std::string& shader) const
	{
		render(glm::vec2(x, y), view, shader);
	}

	void Image::render(const glm::vec2& pos, const glm::mat4& view, const std::string& shader) const
	{
		m_vao.bind();

		std::vector<GLuint> indices = { 0u + (m_flip * 4), 1u + (m_flip * 4), 2u + (m_flip * 4), 2u + (m_flip * 4), 3u + (m_flip * 4), 0u + (m_flip * 4) };

		Renderer::RenderTexture(m_texture.get(), shader, glm::translate(glm::mat4(), glm::vec3(pos, 0.0f)), view, glm::mat4(), indices);
	}

	void Image::renderF(float x, float y, int frame, const glm::mat4& view, const std::string& shader) const
	{
		renderF(glm::vec2(x, y), frame, view, shader);
	}

	void Image::renderF(const glm::vec2& pos, int frame, const glm::mat4& view, const std::string& shader) const
	{
		float fX = (frame % m_columns) * (1.0f / m_columns);
		float fY = 1 - (1.0f / m_rows) - ((frame / m_columns) * (1.0f / m_rows));

		m_vao.bind();

		std::vector<GLuint> indices = { 0u + (m_flip * 4), 1u + (m_flip * 4), 2u + (m_flip * 4), 2u + (m_flip * 4), 3u + (m_flip * 4), 0u + (m_flip * 4) };

		Renderer::RenderTexture(m_texture.get(), shader, glm::translate(glm::mat4(), glm::vec3(pos, 0.0f)), view, glm::mat4(), indices, glm::vec2(fX, fY));
	}
}