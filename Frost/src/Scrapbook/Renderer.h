#pragma once

#include "Util\Singelton.h"
#include "Texture.h"
#include "ResourceManager.h"

namespace sb
{
	class Renderer : private Singleton<Renderer>
	{
	private:
		glm::mat4 m_view;
	public:
		static void Init(float x, float y, float w, float h);
		
		static void RenderTexture(Texture* tex, const glm::vec2& position, const std::string& shader = "shader");

		static glm::mat4 GetView();
	};
}