#pragma once

#include "Graphics.h"
#include "Utility.h"
#include "Maths.h"

#include <GLFW/glfw3.h>

namespace sb
{
	class Scrapbook
	{
	private:
		GLFWwindow* m_window;

		bool m_debug;
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

		void close();

		void setDebugMode(bool b);
		void toggleDebugMode();

		void run();

		virtual void onInput() = 0;
		virtual void onUpdate() = 0;
		virtual void onRender() = 0;
		virtual void onRenderDebug() {}

		void setClearColor(float r, float g, float b, float a);
		void setClearColor(const glm::vec4& color);

		void setTitle(const std::string& title);

		int getWidth() const;
		int getHeight() const;
		GLFWwindow* getContext() const;
	};
}