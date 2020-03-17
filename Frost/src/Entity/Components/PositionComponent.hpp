#pragma once

#include "Component.hpp"

class PositionComponent : public Component
{
private:
	glm::vec2 m_position;

public:
	PositionComponent(Entity* entity, const glm::vec2& position = glm::vec2());

	virtual void SetPosition(const glm::vec2& position);
	virtual glm::vec2 GetPosition() const;
};