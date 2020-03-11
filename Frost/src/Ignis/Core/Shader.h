#pragma once

#include "Api.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct
	{
		GLuint program;
	} ignis_shader;

	ignis_shader* ignisShadervf(const char* vert, const char* frag);
	ignis_shader* ignisShadervgf(const char* vert, const char* geom, const char* frag);
	void ignisDeleteShader(ignis_shader* shader);

	void ignisUseShader(ignis_shader* shader);

	GLint ignisGetUniformLocation(const ignis_shader* shader, const char* name);

	void ignisSetUniform1i(const ignis_shader* shader, const char* name, int value);
	void ignisSetUniform1f(const ignis_shader* shader, const char* name, float value);
	void ignisSetUniform2f(const ignis_shader* shader, const char* name, const float* values);
	void ignisSetUniform3f(const ignis_shader* shader, const char* name, const float* values);
	void ignisSetUniform4f(const ignis_shader* shader, const char* name, const float* values);
	void ignisSetUniformMat2(const ignis_shader* shader, const char* name, const float* values);
	void ignisSetUniformMat3(const ignis_shader* shader, const char* name, const float* values);
	void ignisSetUniformMat4(const ignis_shader* shader, const char* name, const float* values);

	// unchecked location
	void ignisSetUniform1il(GLint location, int value);
	void ignisSetUniform1fl(GLint location, float value);
	void ignisSetUniform2fl(GLint location, const float* values);
	void ignisSetUniform3fl(GLint location, const float* values);
	void ignisSetUniform4fl(GLint location, const float* values);
	void ignisSetUniformMat2l(GLint location, const float* values);
	void ignisSetUniformMat3l(GLint location, const float* values);
	void ignisSetUniformMat4l(GLint location, const float* values);

	// Shader utils
	GLuint ignisCreateShaderProgram(GLenum* types, const char** sources, size_t count);
	GLuint ignisCompileShader(GLenum type, const char* source);

	void ignisPrintShaderLog(GLuint shader);
	void ignisPrintProgramLog(GLuint program);

	const char* ignisGetShaderType(GLenum type);

#ifdef __cplusplus
}
#endif