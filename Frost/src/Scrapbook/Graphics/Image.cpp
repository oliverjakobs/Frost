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
		
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		m_vbo.bind();
		m_vbo.setBufferData(vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
		m_vbo.setAttribPointer(0, 3, 5, 0);
		m_vbo.setAttribPointer(1, 2, 5, 3);

		m_texture = new Texture(src.c_str(), 0);
	}

	Image::Image(const std::string & src, float width, float height, int rows, int columns)
		: m_width(width), m_height(height), m_rows(rows), m_columns(columns)
	{
		float fWidth = 1.0f / m_columns;
		float fHeight = 1.0f / m_rows;


		auto vertices = GetVertices(width, height, fWidth, fHeight);

		m_flip = FLIP_NONE;

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		m_vbo.bind();
		m_vbo.setBufferData(vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
		m_vbo.setAttribPointer(0, 3, 5, 0);
		m_vbo.setAttribPointer(1, 2, 5, 3);

		m_texture = new Texture(src.c_str(), 0);
	}

	Image::~Image()
	{
		if (m_texture != nullptr)
			delete m_texture;
		
		glDeleteVertexArrays(1, &m_vao);
	}

	void Image::setRenderFlip(RenderFlip flip)
	{
		m_flip = flip;
	}
	
	void Image::render(float x, float y, Shader* shader)
	{
		render(glm::vec2(x, y), shader);
	}

	void Image::render(const glm::vec2& pos, Shader* shader)
	{
		glBindVertexArray(m_vao);

		std::vector<GLuint> indices = { 0u + (m_flip * 4), 1u + (m_flip * 4), 2u + (m_flip * 4), 2u + (m_flip * 4), 3u + (m_flip * 4), 0u + (m_flip * 4) };

		Renderer::RenderTexture(m_texture, pos, indices, shader);
	}

	void Image::renderF(float x, float y, int frame, Shader* shader)
	{
		renderF(glm::vec2(x, y), frame, shader);
	}

	void Image::renderF(const glm::vec2& pos, int frame, Shader* shader)
	{
		float fX = (frame % m_columns) * (1.0f / m_columns);
		float fY = 1 - (1.0f / m_rows) - ((frame / m_columns) * (1.0f / m_rows));

		glBindVertexArray(m_vao);

		std::vector<GLuint> indices = { 0u + (m_flip * 4), 1u + (m_flip * 4), 2u + (m_flip * 4), 2u + (m_flip * 4), 3u + (m_flip * 4), 0u + (m_flip * 4) };

		Renderer::RenderTextureF(m_texture, pos, glm::vec2(fX, fY), indices, shader);
	}
}