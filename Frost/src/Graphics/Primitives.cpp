#include "Primitives.h"

#include "Buffer.h"
#include "Shader.h"

#include "Maths/Maths.h"

struct GLRenderLines
{
	enum { MaxVertices = 2 * 512 };

	glm::vec2 m_vertices[MaxVertices];
	glm::vec4 m_colors[MaxVertices];

	int m_count;

	GLuint m_vao;
	GLuint m_vbos[2];

	GLuint m_shader;
	GLint m_viewLocation;

	GLint m_vertexAttribute;
	GLint m_colorAttribute;

	void create()
	{
		const char* vs = \
			"#version 330\n"
			"uniform mat4 view;\n"
			"layout(location = 0) in vec2 v_position;\n"
			"layout(location = 1) in vec4 v_color;\n"
			"out vec4 f_color;\n"
			"void main(void)\n"
			"{\n"
			"	f_color = v_color;\n"
			"	gl_Position = view * vec4(v_position, 0.0f, 1.0f);\n"
			"}\n";

		const char* fs = \
			"#version 330\n"
			"in vec4 f_color;\n"
			"out vec4 color;\n"
			"void main(void)\n"
			"{\n"
			"	color = f_color;\n"
			"}\n";

		m_shader = Shader::CreateShader(vs, fs);
		m_viewLocation = glGetUniformLocation(m_shader, "view");
		m_vertexAttribute = 0;
		m_colorAttribute = 1;

		// Generate
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(2, m_vbos);

		glBindVertexArray(m_vao);
		glEnableVertexAttribArray(m_vertexAttribute);
		glEnableVertexAttribArray(m_colorAttribute);

		// Vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0]);
		glVertexAttribPointer(m_vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1]);
		glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors, GL_DYNAMIC_DRAW);

		// Cleanup
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		m_count = 0;
	}

	void destroy()
	{
		if (m_vao)
		{
			glDeleteVertexArrays(1, &m_vao);
			glDeleteBuffers(2, m_vbos);
			m_vao = 0;
		}
		if (m_shader)
		{
			glDeleteProgram(m_shader);
			m_shader = 0;
		}
	}

	void vertex(const glm::vec2& v, const glm::vec4& c, const glm::mat4& view)
	{
		if (m_count == MaxVertices)
			flush(view);

		m_vertices[m_count] = v;
		m_colors[m_count] = c;
		m_count++;
	}

	void flush(const glm::mat4& view)
	{
		if (m_count == 0)
			return;

		glUseProgram(m_shader);

		glUniformMatrix4fv(m_viewLocation, 1, GL_FALSE, &view[0][0]);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(glm::vec2), m_vertices);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(glm::vec4), m_colors);

		glDrawArrays(GL_LINES, 0, m_count);

		m_count = 0;
	}
};

struct GLRenderTriangles
{
	enum { MaxVertices = 3 * 512 };

	glm::vec2 m_vertices[MaxVertices];
	glm::vec4 m_colors[MaxVertices];

	int m_count;

	GLuint m_vao;
	GLuint m_vbos[2];

	GLuint m_shader;
	GLint m_viewLocation;

	GLint m_vertexAttribute;
	GLint m_colorAttribute;

	void create()
	{
		const char* vs = \
			"#version 330\n"
			"uniform mat4 view;\n"
			"layout(location = 0) in vec2 v_position;\n"
			"layout(location = 1) in vec4 v_color;\n"
			"out vec4 f_color;\n"
			"void main(void)\n"
			"{\n"
			"	f_color = v_color;\n"
			"	gl_Position = view * vec4(v_position, 0.0f, 1.0f);\n"
			"}\n";

		const char* fs = \
			"#version 330\n"
			"in vec4 f_color;\n"
			"out vec4 color;\n"
			"void main(void)\n"
			"{\n"
			"	color = f_color;\n"
			"}\n";

		m_shader = Shader::CreateShader(vs, fs);
		m_viewLocation = glGetUniformLocation(m_shader, "view");
		m_vertexAttribute = 0;
		m_colorAttribute = 1;

		// Generate
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(2, m_vbos);

		glBindVertexArray(m_vao);
		glEnableVertexAttribArray(m_vertexAttribute);
		glEnableVertexAttribArray(m_colorAttribute);

		// Vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0]);
		glVertexAttribPointer(m_vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1]);
		glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors, GL_DYNAMIC_DRAW);

		// Cleanup
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		m_count = 0;
	}

	void destroy()
	{
		if (m_vao)
		{
			glDeleteVertexArrays(1, &m_vao);
			glDeleteBuffers(2, m_vbos);
			m_vao = 0;
		}

		if (m_shader)
		{
			glDeleteProgram(m_shader);
			m_shader = 0;
		}
	}

	void vertex(const glm::vec2& v, const glm::vec4& c, const glm::mat4& view)
	{
		if (m_count == MaxVertices)
			flush(view);

		m_vertices[m_count] = v;
		m_colors[m_count] = c;
		m_count++;
	}

	void flush(const glm::mat4& view)
	{
		if (m_count == 0)
			return;

		glUseProgram(m_shader);
		glUniformMatrix4fv(m_viewLocation, 1, GL_FALSE, &view[0][0]);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(glm::vec2), m_vertices);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(glm::vec4), m_colors);

		glDrawArrays(GL_TRIANGLES, 0, m_count);

		m_count = 0;
	}
};

Primitives::Primitives()
{
	m_lines = NULL;
	m_triangles = NULL;
}

Primitives::~Primitives()
{
	assert(m_lines == NULL);
	assert(m_triangles == NULL);
}

void Primitives::create()
{
	m_lines = new GLRenderLines();
	m_lines->create();

	m_triangles = new GLRenderTriangles();
	m_triangles->create();
}

void Primitives::destroy()
{
	m_lines->destroy();
	delete m_lines;
	m_lines = NULL;

	m_triangles->destroy();
	delete m_triangles;
	m_triangles = NULL;
}

void Primitives::flush(const glm::mat4& view)
{
	m_lines->flush(view);
	m_triangles->flush(view);
}

void Primitives::drawLine(const glm::vec2& start, const glm::vec2& end, const Color& color, const glm::mat4& view) const
{
	m_lines->vertex(start, color, view);
	m_lines->vertex(end, color, view);
}

void Primitives::drawPolygon(const Vertices& vertices, const Color& color, const glm::mat4& view) const
{
	glm::vec2 p1 = vertices.back();

	for (auto& p2 : vertices)
	{
		m_lines->vertex(p1, color, view);
		m_lines->vertex(p2, color, view);
		p1 = p2;
	}
}

void Primitives::fillPolygon(const Vertices& vertices, const Color& color, const glm::mat4& view) const
{
	for (unsigned int i = 1; i < vertices.size() - 1; ++i)
	{
		m_triangles->vertex(vertices[0], color, view);
		m_triangles->vertex(vertices[i], color, view);
		m_triangles->vertex(vertices[i + 1], color, view);
	}
}

void Primitives::fillCircle(const glm::vec2& center, float radius, const Color& color, const glm::mat4& view) const
{
	const float k_segments = 36;
	const float k_increment = 2.0f * M_PI / k_segments;
	float sinInc = sinf(k_increment);
	float cosInc = cosf(k_increment);
	glm::vec2 v0 = center;
	glm::vec2 r1(cosInc, sinInc);
	glm::vec2 v1 = center + radius * r1;

	for (int i = 0; i < k_segments; ++i)
	{
		// Perform rotation to avoid additional trigonometry.
		glm::vec2 r2;
		r2.x = cosInc * r1.x - sinInc * r1.y;
		r2.y = sinInc * r1.x + cosInc * r1.y;
		glm::vec2 v2 = center + radius * r2;
		m_triangles->vertex(v0, color, view);
		m_triangles->vertex(v1, color, view);
		m_triangles->vertex(v2, color, view);
		r1 = r2;
		v1 = v2;
	}

	r1 = glm::vec2(1.0f, 0.0f);
	v1 = center + radius * r1;
	for (int i = 0; i < k_segments; ++i)
	{
		glm::vec2 r2;
		r2.x = cosInc * r1.x - sinInc * r1.y;
		r2.y = sinInc * r1.x + cosInc * r1.y;
		glm::vec2 v2 = center + radius * r2;
		m_lines->vertex(v1, color, view);
		m_lines->vertex(v2, color, view);
		r1 = r2;
		v1 = v2;
	}
}

void Primitives::drawCircle(const glm::vec2& center, float radius, const Color& color, const glm::mat4& view) const
{
	const float k_segments = 36;
	const float k_increment = 2.0f * M_PI / k_segments;

	float sinInc = sinf(k_increment);
	float cosInc = cosf(k_increment);

	glm::vec2 r1(1.0f, 0.0f);
	glm::vec2 v1 = center + radius * r1;

	for (int i = 0; i < k_segments; ++i)
	{
		// Perform rotation to avoid additional trigonometry.
		glm::vec2 r2;
		r2.x = cosInc * r1.x - sinInc * r1.y;
		r2.y = sinInc * r1.x + cosInc * r1.y;
		glm::vec2 v2 = center + radius * r2;
		m_lines->vertex(v1, color, view);
		m_lines->vertex(v2, color, view);
		r1 = r2;
		v1 = v2;
	}
}