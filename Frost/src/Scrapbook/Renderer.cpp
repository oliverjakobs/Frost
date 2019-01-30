#include "Renderer.h"

#include <glad\glad.h>

namespace sb
{
	void Renderer::Init(float x, float y, float w, float h)
	{
		ResourceManager::Load();

		Get()->m_view = glm::ortho(x, w, y, h);
	}

	void Renderer::RenderTexture(Texture* tex, const glm::vec2& position, const std::string& sName)
	{
		Shader* shader = ResourceManager::GetShader(sName);

		shader->use();

		shader->setUniformMat4("projection", glm::mat4(1.0f));
		shader->setUniformMat4("view", GetView());
		shader->setUniformMat4("model", glm::translate(glm::mat4(), glm::vec3(position, 0.0f)));

		tex->bind();

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glm::mat4 Renderer::GetView()
	{
		return Get()->m_view;
	}
}
