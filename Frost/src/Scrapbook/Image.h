#pragma once

#include "Texture.h"
#include "Buffer.h"
#include "Shader.h"

#include <glm\glm.hpp>

namespace sb
{
	class Image
	{
	private:
		Texture* m_texture;
		VBO m_vbo;

		unsigned int m_vao;
	public:
		Image(const std::string& src, float width, float height);
		~Image();

		void render(float x, float y, Shader* shader);
		void render(const glm::vec2& pos, Shader* shader);
	};
}