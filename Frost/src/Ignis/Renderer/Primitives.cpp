#include "Primitives.h"

namespace ignis
{
	static const int MAX_LINES = 2 * 1024;

	struct Primitives2DStorage
	{
		std::shared_ptr<VertexArray> VertexArray;
		std::shared_ptr<Shader> Shader;

		std::vector<glm::vec2> Vertices;
		std::vector<glm::vec4> Colors;
	};

	static Primitives2DStorage* s_renderData;

	void Primitives2D::Init(const std::shared_ptr<Shader>& shader)
	{
		s_renderData = new Primitives2DStorage();

		s_renderData->VertexArray = std::make_shared<VertexArray>();

		s_renderData->VertexArray->AddArrayBuffer(sizeof(glm::vec2) * MAX_LINES, nullptr, GL_DYNAMIC_DRAW, { {GL_FLOAT, 2} });
		s_renderData->VertexArray->AddArrayBuffer(sizeof(glm::vec4) * MAX_LINES, nullptr, GL_DYNAMIC_DRAW, { {GL_FLOAT, 4} });

		s_renderData->Shader = shader;
	}

	void Primitives2D::Destroy()
	{
		delete s_renderData;
	}

	void Primitives2D::Start(const glm::mat4& viewProjection)
	{
		s_renderData->Shader->Use();
		s_renderData->Shader->SetUniformMat4("u_ViewProjection", viewProjection);
	}

	void Primitives2D::Flush()
	{
		if (s_renderData->Vertices.empty())
			return;

		s_renderData->VertexArray->Bind();
		s_renderData->VertexArray->GetArrayBuffer(0)->BufferSubData(0, s_renderData->Vertices.size() * sizeof(glm::vec2), &s_renderData->Vertices[0]);
		s_renderData->VertexArray->GetArrayBuffer(1)->BufferSubData(0, s_renderData->Colors.size() * sizeof(glm::vec4), &s_renderData->Colors[0]);

		s_renderData->Shader->Use();

		glDrawArrays(GL_LINES, 0, s_renderData->Vertices.size());

		s_renderData->Vertices.clear();
		s_renderData->Colors.clear();
	}

	void Primitives2D::Vertex(const glm::vec2& v, const color& c)
	{
		if (s_renderData->Vertices.size() >= MAX_LINES)
			Flush();

		s_renderData->Vertices.push_back(v);
		s_renderData->Colors.push_back(c);
	}

	void Primitives2D::DrawLine(float x1, float y1, float x2, float y2, const color& c)
	{
		DrawLine(glm::vec2(x1, y1), glm::vec2(x2, y2), c);
	}

	void Primitives2D::DrawLine(const glm::vec2& start, const glm::vec2& end, const color& c)
	{
		Vertex(start, c);
		Vertex(end, c);
	}

	void Primitives2D::DrawRect(float x, float y, float w, float h, const color& c)
	{
		std::vector<glm::vec2> vertices =
		{
			glm::vec2(x, y),
			glm::vec2(x + w, y),
			glm::vec2(x + w, y + h),
			glm::vec2(x, y + h)
		};

		DrawPolygon(vertices, c);
	}

	void Primitives2D::DrawRect(const glm::vec2& pos, const glm::vec2& dim, const color& c)
	{
		DrawRect(pos.x, pos.y, dim.x, dim.y, c);
	}

	void Primitives2D::DrawPolygon(const std::vector<glm::vec2>& vertices, const color& c)
	{
		glm::vec2 p1 = vertices.back();

		for (auto& p2 : vertices)
		{
			Vertex(p1, c);
			Vertex(p2, c);
			p1 = p2;
		}
	}

	void Primitives2D::DrawCircle(const glm::vec2& center, float radius, const color& c)
	{
		const float PI = 3.14159265359f;

		const float k_segments = 36;
		const float k_increment = 2.0f * PI / k_segments;

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
			Vertex(v1, c);
			Vertex(v2, c);
			r1 = r2;
			v1 = v2;
		}
	}
}