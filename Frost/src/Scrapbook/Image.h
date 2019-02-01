#pragma once

#include "Renderer.h"
#include "Buffer.h"

#include <glm\glm.hpp>

namespace sb
{
	class Image
	{
	private:
		Texture* m_texture;
		VBO m_vbo;

		unsigned int m_vao;

		float m_width;
		float m_height;
	public:
		Image(const std::string& src, float width, float height);
		~Image();

		void render(float x, float y, Shader* shader);
		void render(const glm::vec2& pos, Shader* shader);

		inline float getWidth() const { return m_width; }
		inline float getHeight() const { return m_height; }
	};
}