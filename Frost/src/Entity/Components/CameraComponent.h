#pragma once

#include "Component.h"
#include "Maths/Maths.h"

class CameraComponent : public Component
{
private:
	Rect m_constraint;
	glm::vec2 m_offset;

public:
	CameraComponent(const Rect& constraint, const glm::vec2& offset);

	Rect GetConstraint() const;
	glm::vec2 GetOffset() const;

	void OnUpdate() override;
	void OnRender() override;
};
