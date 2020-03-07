#include "Shader.h"

#include <fstream>

namespace ignis
{
	static GLuint s_activeProgram = 0;

	static std::string read_file(const std::string& path)
	{
		std::ifstream ifs(path);

		if (ifs) return std::string(std::istreambuf_iterator<char>(ifs.rdbuf()), std::istreambuf_iterator<char>());

		return std::string();
	}

	Shader::Shader(const std::string& vert, const std::string& frag)
	{
		m_program = CreateShaderProgram({ 
			{ GL_VERTEX_SHADER, read_file(vert) }, 
			{ GL_FRAGMENT_SHADER, read_file(frag) }
			});
	}

	Shader::Shader(const std::string& vert, const std::string& geom, const std::string& frag)
	{
		m_program = CreateShaderProgram({
			{ GL_VERTEX_SHADER, read_file(vert) },
			{ GL_GEOMETRY_SHADER, read_file(geom) },
			{ GL_FRAGMENT_SHADER, read_file(frag) }
			});
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_program);
	}

	void Shader::Use()
	{
		// check if program handle refer to an object generated by OpenGL
		IGNIS_ASSERT(glIsProgram(m_program));

		if (s_activeProgram != m_program)
		{
			glUseProgram(m_program);
			s_activeProgram = m_program;
		}
	}

	void Shader::SetUniform1i(const char* name, int value) const
	{
		int location = GetUniformLocation(name);

		if (location < 0)
			_ignisErrorCallback(ignisErrorLevel::Warn, "[SHADER] Uniform %s not found", name);
		else
			glUniform1i(location, value);
	}

	void Shader::SetUniform1f(const char* name, float value) const
	{
		int location = GetUniformLocation(name);

		if (location < 0)
			_ignisErrorCallback(ignisErrorLevel::Warn, "[SHADER] Uniform %s not found", name);
		else
			glUniform1f(location, value);
	}

	void Shader::SetUniform2f(const char* name, const glm::vec2& vector) const
	{
		int location = GetUniformLocation(name);

		if (location < 0)
			_ignisErrorCallback(ignisErrorLevel::Warn, "[SHADER] Uniform %s not found", name);
		else
			glUniform2fv(location, 1, &vector[0]);
	}

	void Shader::SetUniform3f(const char* name, const glm::vec3& vector) const
	{
		int location = GetUniformLocation(name);

		if (location < 0)
			_ignisErrorCallback(ignisErrorLevel::Warn, "[SHADER] Uniform %s not found", name);
		else
			glUniform3fv(location, 1, &vector[0]);
	}

	void Shader::SetUniform4f(const char* name, const glm::vec4& vector) const
	{
		int location = GetUniformLocation(name);

		if (location < 0)
			_ignisErrorCallback(ignisErrorLevel::Warn, "[SHADER] Uniform %s not found", name);
		else
			glUniform4fv(location, 1, &vector[0]);
	}

	void Shader::SetUniformMat2(const char* name, const glm::mat4& matrix) const
	{
		int location = GetUniformLocation(name);

		if (location < 0)
			_ignisErrorCallback(ignisErrorLevel::Warn, "[SHADER] Uniform %s not found", name);
		else
			glUniformMatrix2fv(location, 1, GL_FALSE, &matrix[0][0]);
	}

	void Shader::SetUniformMat3(const char* name, const glm::mat4& matrix) const
	{
		int location = GetUniformLocation(name);

		if (location < 0)
			_ignisErrorCallback(ignisErrorLevel::Warn, "[SHADER] Uniform %s not found", name);
		else
			glUniformMatrix3fv(location, 1, GL_FALSE, &matrix[0][0]);
	}

	void Shader::SetUniformMat4(const char* name, const glm::mat4& matrix) const
	{
		int location = GetUniformLocation(name);

		if (location < 0)
			_ignisErrorCallback(ignisErrorLevel::Warn, "[SHADER] Uniform %s not found", name);
		else
			glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
	}

	int Shader::GetUniformLocation(const char* name) const
	{
		return glGetUniformLocation(m_program, name);
	}

	void Shader::SetUniform1i(int location, int value) const
	{
		glUniform1i(location, value);
	}

	void Shader::SetUniform1f(int location, float value) const
	{
		glUniform1f(location, value);
	}

	void Shader::SetUniform2f(int location, const glm::vec2& vector) const
	{
		glUniform2fv(location, 1, &vector[0]);
	}

	void Shader::SetUniform3f(int location, const glm::vec3& vector) const
	{
		glUniform3fv(location, 1, &vector[0]);
	}

	void Shader::SetUniform4f(int location, const glm::vec4& vector) const
	{
		glUniform4fv(location, 1, &vector[0]);
	}

	void Shader::SetUniformMat2(int location, const glm::mat4& matrix) const
	{
		glUniformMatrix2fv(location, 1, GL_FALSE, &matrix[0][0]);
	}

	void Shader::SetUniformMat3(int location, const glm::mat4& matrix) const
	{
		glUniformMatrix3fv(location, 1, GL_FALSE, &matrix[0][0]);
	}

	void Shader::SetUniformMat4(int location, const glm::mat4& matrix) const
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
	}

	// Shader utils
	GLuint CreateShaderProgram(std::map<GLenum, const std::string&> sources)
	{
		GLuint program = glCreateProgram();
		std::vector<GLuint> attachedShader;

		for (auto& [type, src] : sources)
		{
			GLuint shader = CompileShader(type, src);

			if (shader == 0)
			{
				glDeleteProgram(program);
				return 0;
			}

			glAttachShader(program, shader);
			attachedShader.push_back(shader);
		}

		glLinkProgram(program);

		for (auto& shader : attachedShader)
		{
			glDeleteShader(shader);
			glDetachShader(program, shader);
		}

		GLint result = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &result);
		if (result == GL_FALSE)
		{
			_ignisErrorCallback(ignisErrorLevel::Error, "[SHADER] Linking Error");
			_ignisErrorCallback(ignisErrorLevel::Error, "[SHADER] %s", GetProgramLog(program).c_str());
			glDeleteProgram(program);
			return 0;
		}

		glValidateProgram(program);

		result = GL_FALSE;
		glGetProgramiv(program, GL_VALIDATE_STATUS, &result);
		if (result == GL_FALSE)
		{
			_ignisErrorCallback(ignisErrorLevel::Error, "[SHADER] Validating Error");
			_ignisErrorCallback(ignisErrorLevel::Error, "[SHADER] %s", GetProgramLog(program).c_str());
			glDeleteProgram(program);

			return 0;
		}

		return program;
	}

	GLuint CompileShader(GLenum type, const std::string& source)
	{
		if (source.empty())
		{
			_ignisErrorCallback(ignisErrorLevel::Error, "[SHADER] Shader source is missing for %s", GetShaderType(type));
			return 0;
		}

		GLuint shader = glCreateShader(type);

		const char* data = source.c_str();
		glShaderSource(shader, 1, &data, nullptr);
		glCompileShader(shader);

		GLint result = GL_FALSE;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			_ignisErrorCallback(ignisErrorLevel::Error, "[SHADER] Compiling Error (%s)", GetShaderType(type));
			_ignisErrorCallback(ignisErrorLevel::Error, "[SHADER] %s", GetShaderLog(shader).c_str());
			glDeleteShader(shader);

			return 0;
		}

		return shader;
	}

	std::string GetShaderLog(GLuint object)
	{
		GLint logLength = 0;

		if (!glIsShader(object))
			return "Failed to log: Object is not a shader";

		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &logLength);

		std::vector<GLchar> log(logLength);

		glGetShaderInfoLog(object, logLength, &logLength, &log[0]);

		return std::string(&log[0], logLength - 1);
	}

	GLint GetShaderLog(GLuint object, GLchar* log)
	{
		GLint log_length = 0;

		if (!glIsShader(object))
		{
			log = "Failed to log: Object is not a shader";
			return log_length;
		}

		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);

		std::vector<GLchar> log_vec(log_length);

		glGetShaderInfoLog(object, log_length, &log_length, &log_vec[0]);
		*log = log_vec[0];

		return log_length;
	}

	std::string GetProgramLog(GLuint object)
	{
		GLint logLength = 0;

		if (!glIsProgram(object))
			return "Failed to log: Object is not a program";

		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &logLength);

		std::vector<GLchar> log(logLength);

		glGetProgramInfoLog(object, logLength, &logLength, &log[0]);

		return std::string(&log[0], logLength - 1);
	}

	const char* GetShaderType(GLenum type)
	{
		switch (type)
		{
		case GL_COMPUTE_SHADER:
			return "GL_COMPUTE_SHADER";
		case GL_VERTEX_SHADER:
			return "GL_VERTEX_SHADER";
		case GL_TESS_CONTROL_SHADER:
			return "GL_TESS_CONTROL_SHADER";
		case GL_TESS_EVALUATION_SHADER:
			return "GL_TESS_EVALUATION_SHADER";
		case GL_GEOMETRY_SHADER:
			return "GL_GEOMETRY_SHADER";
		case GL_FRAGMENT_SHADER:
			return "GL_FRAGMENT_SHADER";
		}

		return "INVALID_SHADER_TYPE";
	}
}