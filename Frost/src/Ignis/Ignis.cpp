#include "Ignis.h"

#include <glm/gtc/matrix_transform.hpp>

#include <sstream> 

namespace ignis
{
	void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		// ignore non-significant error/warning codes
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		_ignisErrorCallback(ignisErrorLevel::Error, "[OpenGL] Debug output (%d):", id);
		_ignisErrorCallback(ignisErrorLevel::Error, "[OpenGL] %s", message);

		switch (source)
		{
		case GL_DEBUG_SOURCE_API:				_ignisErrorCallback(ignisErrorLevel::Error, "[OpenGL] Source: API"); break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		_ignisErrorCallback(ignisErrorLevel::Error, "[OpenGL] Source: Window System"); break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:	_ignisErrorCallback(ignisErrorLevel::Error, "[OpenGL] Source: Shader Compiler"); break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:		_ignisErrorCallback(ignisErrorLevel::Error, "[OpenGL] Source: Third Party"); break;
		case GL_DEBUG_SOURCE_APPLICATION:		_ignisErrorCallback(ignisErrorLevel::Error, "[OpenGL] Source: Application"); break;
		case GL_DEBUG_SOURCE_OTHER:				_ignisErrorCallback(ignisErrorLevel::Error, "[OpenGL] Source: Other"); break;
		}

		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:				_ignisErrorCallback(ignisErrorLevel::Error, "[OpenGL] Type: Error"); break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	_ignisErrorCallback(ignisErrorLevel::Error, "[OpenGL] Type: Deprecated Behaviour"); break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	_ignisErrorCallback(ignisErrorLevel::Error, "[OpenGL] Type: Undefined Behaviour"); break;
		case GL_DEBUG_TYPE_PORTABILITY:			_ignisErrorCallback(ignisErrorLevel::Error, "[OpenGL] Type: Portability"); break;
		case GL_DEBUG_TYPE_PERFORMANCE:			_ignisErrorCallback(ignisErrorLevel::Error, "[OpenGL] Type: Performance"); break;
		case GL_DEBUG_TYPE_MARKER:				_ignisErrorCallback(ignisErrorLevel::Error, "[OpenGL] Type: Marker"); break;
		case GL_DEBUG_TYPE_PUSH_GROUP:			_ignisErrorCallback(ignisErrorLevel::Error, "[OpenGL] Type: Push Group"); break;
		case GL_DEBUG_TYPE_POP_GROUP:			_ignisErrorCallback(ignisErrorLevel::Error, "[OpenGL] Type: Pop Group"); break;
		case GL_DEBUG_TYPE_OTHER:				_ignisErrorCallback(ignisErrorLevel::Error, "[OpenGL] Type: Other"); break;
		}

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:			_ignisErrorCallback(ignisErrorLevel::Error, "[OpenGL] Severity: high"); break;
		case GL_DEBUG_SEVERITY_MEDIUM:			_ignisErrorCallback(ignisErrorLevel::Error, "[OpenGL] Severity: medium"); break;
		case GL_DEBUG_SEVERITY_LOW:				_ignisErrorCallback(ignisErrorLevel::Error, "[OpenGL] Severity: low"); break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:	_ignisErrorCallback(ignisErrorLevel::Error, "[OpenGL] Severity: notification"); break;
		}
	}

	static glm::mat4 s_screenMat = glm::mat4(1.0f);

	bool ignisInit(int width, int height)
	{
		s_screenMat = glm::ortho(0.0f, (float)width, (float)height, 0.0f);

		return true;
	}

	bool ignisLoadGL(bool debug)
	{
		// loading glad
		if (!gladLoadGL())
		{
			_ignisErrorCallback(ignisErrorLevel::Error, "[GLAD] Failed to initialize GLAD");
			return false;
		}

		if (debug)
		{
			//Set up opengl debug output
			GLint flags, minor, major;
			glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
			glGetIntegerv(GL_MINOR_VERSION, &minor);
			glGetIntegerv(GL_MAJOR_VERSION, &major);

			if ((flags & GL_CONTEXT_FLAG_DEBUG_BIT) && (major >= 4 && minor >= 3))
			{
				glEnable(GL_DEBUG_OUTPUT);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				glDebugMessageCallback(glDebugOutput, nullptr);
				glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			}
			else
			{
				_ignisErrorCallback(ignisErrorLevel::Error, "[OpenGL] Could not create debug context");
			}
		}

		return true;
	}

	void ignisViewport(int x, int y, int w, int h)
	{
		s_screenMat = glm::ortho((float)x, (float)w, (float)h, (float)y);

		glViewport(x, y, w, h);
	}

	const glm::mat4& ignisScreenMat()
	{
		return s_screenMat;
	}
}