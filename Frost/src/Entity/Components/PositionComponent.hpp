#pragma once

#include "Component.hpp"

class PositionComponent : public Component
{
private:
	vec2 m_position;

public:
	PositionComponent(Entity* entity, const vec2 position);

	virtual void SetPosition(const vec2 position);
	virtual vec2 GetPosition() const;
};