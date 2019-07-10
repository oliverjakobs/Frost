#include "View.h"

void View::SetScreen(float width, float height)
{
	Get().m_screenMat = glm::ortho(0.0f, width, 0.0f, height);
}

void View::Set(float x, float y)
{
	Set(x, y, Get().m_w, Get().m_h);
}

void View::Set(float x, float y, float width, float height)
{
	Get().m_x = x;
	Get().m_y = y;
	Get().m_w = width;
	Get().m_h = height;
	Get().m_mat = glm::ortho(x, x + width, y, y + height);
}

void View::SetCenter(float x, float y)
{
	Set(x - Get().m_w / 2.0f, y - Get().m_h / 2.0f);
}

void View::SetCenter(const glm::vec2& center)
{
	SetCenter(center.x, center.y);
}

void View::SetCenter(float x, float y, Rect constraint)
{
	x -= Get().m_w / 2.0f;
	y -= Get().m_h / 2.0f;

	if (x < constraint.pos.x)
		x = constraint.pos.x;

	if (x + Get().m_w > constraint.pos.x + constraint.dim.x)
		x = constraint.pos.x + constraint.dim.x - Get().m_w;

	if (y < constraint.pos.y)
		y = constraint.pos.y;

	if (y + Get().m_h > constraint.pos.y + constraint.dim.y)
		y = constraint.pos.y + constraint.dim.y - Get().m_h;

	Set(x, y);
}

void View::SetCenter(const glm::vec2& center, Rect constraint)
{
	SetCenter(center.x, center.y, constraint);
}

void View::Resize(float w, float h)
{
	Set(GetX(), GetY(), w, h);
}

glm::mat4 View::GetMat()
{
	return Get().m_mat;
}

glm::mat4 View::GetScreenMat()
{
	return Get().m_screenMat;
}

float View::GetX()
{
	return Get().m_x;
}

float View::GetY()
{
	return Get().m_y;
}

float View::GetWidth()
{
	return Get().m_w;
}

float View::GetHeight()
{
	return Get().m_h;
}
