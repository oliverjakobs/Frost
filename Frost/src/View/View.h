#pragma once

#include "Maths/Maths.h"
#include "Obelisk/Singelton.h"

class View : private Singleton<View>
{
private:
	float m_x;
	float m_y;
	float m_w;
	float m_h;

	glm::mat4 m_mat;
	glm::mat4 m_screenMat;
public:
	static void SetScreen(float width, float height);

	static void Set(float x, float y);
	static void Set(float x, float y, float width, float height);
	static void SetCenter(float x, float y);
	static void SetCenter(const glm::vec2& center);
	static void SetCenter(float x, float y, Rect constraint);
	static void SetCenter(const glm::vec2& center, Rect constraint);

	static void Resize(float w, float h);

	static glm::mat4 GetMat();
	static glm::mat4 GetScreenMat();

	static float GetX();
	static float GetY();
	static float GetWidth();
	static float GetHeight();
};