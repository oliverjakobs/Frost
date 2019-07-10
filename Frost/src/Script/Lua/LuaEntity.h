#pragma once

#include "Types.h"

class Entity;
class LuaEntity
{
private:
	Entity* m_entity;

public:
	LuaEntity(Entity* entity);

	// base
	void SetPosition(const glm::vec2& pos);
	void SetDimension(const glm::vec2& dim);
	void SetDirection(const std::string& direction);

	glm::vec2 GetPosition();
	glm::vec2 GetDimension();
	std::string GetDirection();

	// physics
	void SetVelocity(const glm::vec2& vel);
	glm::vec2 GetVelocity();

	bool CollidesBottom();
	bool CollidesTop();
	bool CollidesLeft();
	bool CollidesRight();

	void Drop();

	// graphics
	void PlayAnimation(const std::string& animation);
	void SetRenderFlip(const std::string& flip);

	// view
	void CenterView(const glm::vec2& offset, bool lock);
};