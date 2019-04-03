#pragma once

#include "Maths/Maths.h"
#include "Utility/utils.h"

class Shader
{
private:
	unsigned int m_program;
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	static unsigned int CreateShader(const std::string& vertSrc, const std::string& fragSrc);
	static unsigned int CompileShader(unsigned int type, const char* source);
	static void PrintLog(unsigned int object);

	void use();

	void setUniform1i(const std::string& name, int value) const;
	void setUniform1f(const std::string& name, float value) const;
	void setUniform2f(const std::string& name, const glm::vec2& vector) const;
	void setUniform3f(const std::string& name, const glm::vec3& vector) const;
	void setUniform4f(const std::string& name, const glm::vec4& vector) const;
	void setUniformMat2(const std::string& name, const glm::mat4& matrix) const;
	void setUniformMat3(const std::string& name, const glm::mat4& matrix) const;
	void setUniformMat4(const std::string& name, const glm::mat4& matrix) const;
};