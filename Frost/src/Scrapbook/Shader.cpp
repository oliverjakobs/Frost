#include "Shader.h"

#include <glad\glad.h>

#include <fstream>
#include <sstream>
#include <iostream>

namespace sb
{
	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		// vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		// print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		};

		// similiar for Fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		// print compile errors if any
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		};

		// shader Program
		m_program = glCreateProgram();
		glAttachShader(m_program, vertex);
		glAttachShader(m_program, fragment);
		glLinkProgram(m_program);
		// print linking errors if any
		glGetProgramiv(m_program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(m_program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		// delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	Shader::~Shader()
	{
		glDeleteShader(m_program);
	}

	void Shader::use()
	{
		glUseProgram(m_program);
	}

	void Shader::setUniform1i(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_program, name.c_str()), value);
	}

	void Shader::setUniform1f(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_program, name.c_str()), value);
	}

	void Shader::setUniform2f(const std::string& name, const glm::vec2& vector) const
	{
		glUniform2fv(glGetUniformLocation(m_program, name.c_str()), 1, &vector[0]);
	}

	void Shader::setUniform3f(const std::string& name, const glm::vec3& vector) const
	{
		glUniform3fv(glGetUniformLocation(m_program, name.c_str()), 1, &vector[0]);
	}

	void Shader::setUniform4f(const std::string& name, const glm::vec4& vector) const
	{
		glUniform4fv(glGetUniformLocation(m_program, name.c_str()), 1, &vector[0]);
	}

	void Shader::setUniformMat2(const std::string& name, const glm::mat4& matrix) const
	{
		glUniformMatrix2fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
	}

	void Shader::setUniformMat3(const std::string& name, const glm::mat4& matrix) const
	{
		glUniformMatrix3fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
	}

	void Shader::setUniformMat4(const std::string& name, const glm::mat4& matrix) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
	}
}