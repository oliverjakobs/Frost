#include "Primitives.hpp"

#define PRIMITIVES2D_MAX_LINES		(2 * 1024)
#define PRIMITIVES2D_VERTEX_SIZE	(2 + 4)

#define PRIMITIVES2D_BUFFER_SIZE	(PRIMITIVES2D_VERTEX_SIZE * PRIMITIVES2D_MAX_LINES)

struct Primitives2DStorage
{
	ignis_vertex_array* VertexArray;
	ignis_shader* Shader;

	float* Vertices;
	GLsizei Vertex_Count;
};

static Primitives2DStorage* s_renderData;

void Primitives2D::Init(ignis_shader* shader)
{
	s_renderData = new Primitives2DStorage();

	s_renderData->VertexArray = ignisGenerateVertexArray();

	ignis_buffer_element layout[] =
	{
		{GL_FLOAT, 2, 0},
		{GL_FLOAT, 4, 0}
	};

	ignisAddArrayBufferLayout(s_renderData->VertexArray, PRIMITIVES2D_BUFFER_SIZE * sizeof(float), NULL, GL_DYNAMIC_DRAW, layout, 2);

	s_renderData->Vertices = (float*)malloc(PRIMITIVES2D_BUFFER_SIZE * sizeof(float));
	s_renderData->Vertex_Count = 0;

	s_renderData->Shader = shader;
}

void Primitives2D::Destroy()
{
	free(s_renderData->Vertices);

	ignisDeleteShader(s_renderData->Shader);
	delete s_renderData;
}

void Primitives2D::Start(const glm::mat4& viewProjection)
{
	ignisUseShader(s_renderData->Shader);
	ignisSetUniformMat4(s_renderData->Shader, "u_ViewProjection", &viewProjection[0][0]);
}

void Primitives2D::Flush()
{
	if (s_renderData->Vertex_Count == 0)
		return;

	ignisBindVertexArray(s_renderData->VertexArray);
	ignisBufferSubData(&s_renderData->VertexArray->array_buffers[0], 0, s_renderData->Vertex_Count * sizeof(float), s_renderData->Vertices);

	ignisUseShader(s_renderData->Shader);

	glDrawArrays(GL_LINES, 0, s_renderData->Vertex_Count / PRIMITIVES2D_VERTEX_SIZE);

	s_renderData->Vertex_Count = 0;
}

void Primitives2D::Vertex(float x, float y, const ignis_color_rgba& color)
{
	if (s_renderData->Vertex_Count >= PRIMITIVES2D_BUFFER_SIZE)
		Flush();

	s_renderData->Vertices[s_renderData->Vertex_Count++] = x;
	s_renderData->Vertices[s_renderData->Vertex_Count++] = y;

	s_renderData->Vertices[s_renderData->Vertex_Count++] = color.r;
	s_renderData->Vertices[s_renderData->Vertex_Count++] = color.g;
	s_renderData->Vertices[s_renderData->Vertex_Count++] = color.b;
	s_renderData->Vertices[s_renderData->Vertex_Count++] = color.a;
}

void Primitives2D::DrawLine(float x1, float y1, float x2, float y2, const ignis_color_rgba& color)
{
	Vertex(x1, y1, color);
	Vertex(x2, y2, color);
}

void Primitives2D::DrawRect(float x, float y, float w, float h, const ignis_color_rgba& color)
{
	float vertices[] =
	{
		x + 0, y + 0,
		x + w, y + 0,
		x + w, y + h,
		x + 0, y + h
	};

	DrawPolygon(vertices, 8, color);
}

void Primitives2D::DrawPolygon(float* vertices, size_t count, const ignis_color_rgba& color)
{
	if (!vertices || count < 2) return;

	float p1x = vertices[count - 2];
	float p1y = vertices[count - 1];

	size_t i = 0;
	while (i < count - 1)
	{
		float p2x = vertices[i++];
		float p2y = vertices[i++];

		Vertex(p1x, p1y, color);
		Vertex(p2x, p2y, color);

		p1x = p2x;
		p1y = p2y;
	}
}

void Primitives2D::DrawCircle(float x, float y, float radius, const ignis_color_rgba& color)
{
	const float PI = 3.14159265359f;

	const float k_segments = 36;
	const float k_increment = 2.0f * PI / k_segments;

	float sinInc = sinf(k_increment);
	float cosInc = cosf(k_increment);

	float r1x = 1.0f;
	float r1y = 0.0f;

	float v1x = x + radius * r1x;
	float v1y = y + radius * r1y;

	for (int i = 0; i < k_segments; ++i)
	{
		// Perform rotation to avoid additional trigonometry.
		float r2x = cosInc * r1x - sinInc * r1y;
		float r2y = sinInc * r1x + cosInc * r1y;

		float v2x = x + radius * r2x;
		float v2y = y + radius * r2y;

		Vertex(v1x, v1y, color);
		Vertex(v2x, v2y, color);

		r1x = r2x;
		r1y = r2y;
		v1x = v2x;
		v1y = v2y;
	}
}