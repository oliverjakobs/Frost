#include "Tilemap.h"
#include "Scrapbook\Framebuffer.h"

#include <iostream>

using namespace sb;


class TileMapExample : public Scrapbook
{
private:
	TileMap* map;

	Shader* shader;

	FrameBuffer* frameBuffer;
public:
	TileMapExample() : Scrapbook("TileMap", 1200, 800)
	{
		setClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glfwSwapInterval(0);

		// ------------------------------------------------
		shader = new Shader("res/shader/shader.vert", "res/shader/shader.frag");

		frameBuffer = new FrameBuffer(m_data.width, m_data.height);

		std::vector<int> m =
		{
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
			0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
			0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
			0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		};

		map = new TileMap("res/images/box.png", 24, 16, 50.0f, m);
	}

	void onInput() override
	{
		if (glfwGetKey(getContext(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(getContext(), true);

		if (glfwGetKey(getContext(), GLFW_KEY_1) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (glfwGetKey(getContext(), GLFW_KEY_2) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		if (glfwGetKey(getContext(), GLFW_KEY_3) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

		if (glfwGetKey(getContext(), GLFW_KEY_F1) == GLFW_PRESS)
			glViewport(0.0f, 0.0f, getWidth(), getHeight());

		if (glfwGetKey(getContext(), GLFW_KEY_F2) == GLFW_PRESS)
			glViewport(0.0f, 0.0f, getWidth() / 2.0f, getHeight() / 2.0f);
	}

	void onUpdate() override
	{
		setTitle(m_data.title + "  |  " + std::to_string(m_timer.getFPS()));
	}

	void onRender() override
	{
		frameBuffer->bind();

		// camera/view transformation
		glm::mat4 projection = glm::mat4();
		glm::mat4 view = glm::ortho(0.0f, (float)m_data.width, 0.0f, (float)m_data.height);

		// render box
		shader->use();

		shader->setUniformMat4("projection", projection);
		shader->setUniformMat4("view", view);

		map->onRender(shader);

		frameBuffer->unbind();

		shader->use();
		shader->setUniformMat4("projection", glm::mat4(1.0f));
		shader->setUniformMat4("view", glm::mat4(1.0f));
		shader->setUniformMat4("model", glm::mat4(1.0f));

		frameBuffer->render();
	}
};

int main()
{
	TileMapExample game;

	game.run();

	return 0;
}