#pragma once

#include "Api.h"

namespace ignis
{
	class Shader
	{
	protected:
		GLuint m_program;

		Shader() : m_program(0) {};

	public:
		Shader(const char* vert, const char* frag);
		Shader(const char* vert, const char* geom, const char* frag);
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
	GLuint CreateShaderProgram(GLenum* types, const char** sources, size_t count);
	GLuint CompileShader(GLenum type, const char* source);

	void PrintShaderLog(GLuint shader);
	void PrintProgramLog(GLuint program);

	const char* GetShaderType(GLenum type);
}