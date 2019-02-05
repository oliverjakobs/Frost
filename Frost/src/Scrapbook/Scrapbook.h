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

		void enableVsync(bool b);

		void run();

		virtual void onInput() = 0;
		virtual void onUpdate() = 0;
		virtual void onRender() = 0;
		virtual void onRenderDebug() {}
		
		void setTitle(const std::string& title);

		int getWidth() const;
		int getHeight() const;
		GLFWwindow* getContext() const;
	};
}