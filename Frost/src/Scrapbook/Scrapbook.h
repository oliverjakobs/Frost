#pragma once

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "Buffer.h"

#include "Image.h"

#include "Timer.h"

namespace sb
{
	class Scrapbook
	{
	private:
		GLFWwindow* m_window;
	protected:
		struct WindowData
		{
			std::string title;

			int width;
			int height;
		} m_data;
	public:
		Scrapbook(const std::string& title, int width, int height);
		~Scrapbook();

		void run();

		virtual void onInput() = 0;
		virtual void onUpdate() = 0;
		virtual void onRender() = 0;

		void setClearColor(float r, float g, float b, float a);
		void setClearColor(const glm::vec4& color);

		void setTitle(const std::string& title);

		int getWidth() const;
		int getHeight() const;
		GLFWwindow* getContext() const;
	};
}