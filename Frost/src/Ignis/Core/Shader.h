#pragma once

#include "Ignis/glad/glad.h"

#include "Api.h"

namespace ignis
{
	class Shader
	{
	protected:
		GLuint m_program;

		Shader() : m_program(0) {};

	public:
		Shader(const std::string& vert, const std::string& frag);
		Shader(const std::string& vert, const std::string& geom, const std::string& frag);
		virtual ~Shader();

		void Use();

		void SetUniform1i(const char* name, int value) const;
		void SetUniform1f(const char* name, float value) const;
		void SetUniform2f(const char* name, const float* values) const;
		void SetUniform3f(const char* name, const float* values) const;
		void SetUniform4f(const char* name, const float* values) const;
		void SetUniformMat2(const char* name, const float* values) const;
		void SetUniformMat3(const char* name, const float* values) const;
		void SetUniformMat4(const char* name, const float* values) const;

		int GetUniformLocation(const char* name) const;

		void SetUniform1i(int location, int value) const;
		void SetUniform1f(int location, float value) const;
		void SetUniform2f(int location, const float* values) const;
		void SetUniform3f(int location, const float* values) const;
		void SetUniform4f(int location, const float* values) const;
		void SetUniformMat2(int location, const float* values) const;
		void SetUniformMat3(int location, const float* values) const;
		void SetUniformMat4(int location, const float* values) const;
	};

	// Shader utils
	GLuint CreateShaderProgram(std::map<GLenum, const std::string&> sources);
	GLuint CompileShader(GLenum type, const std::string& source);

	void PrintShaderLog(GLuint shader);
	void PrintProgramLog(GLuint program);

	const char* GetShaderType(GLenum type);
}