#pragma once

#include "Shader.h"
#include "Buffer.h"
#include "Texture.h"

namespace sb
{
	enum RenderFlip
	{
		FLIP_NONE = 0,
		FLIP_HORIZONTAL = 1,
		FLIP_VERTICAL = 2
	};

	class Image
	{
	private:
		Texture* m_texture;
		VBO m_vbo;

		unsigned int m_vao;

		RenderFlip m_flip;

		int m_columns;
		int m_rows;

		float m_width;
		float m_height;
	public:
		Image(const std::string& src, float width, float height);
		Image(const std::string& src, float width, float height, int rows, int columns);
		~Image();

		void setRenderFlip(RenderFlip flip);

		void render(float x, float y, Shader* shader);
		void render(const glm::vec2& pos, Shader* shader);

		void renderF(float x, float y, int frame, Shader* shader);
		void renderF(const glm::vec2& pos, int frame, Shader* shader);

		inline float getWidth() const { return m_width; }
		inline float getHeight() const { return m_height; }
	};
}