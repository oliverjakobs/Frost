#pragma once

#include "ecs/ecsRegistry.h"
#include "Maths/Maths.h"

class TransformComponent : public ecs::Component
{
public:
	GENERATE_COMPONENT_TYPE_ID(TransformComponent)

	glm::vec2 m_position;

	TransformComponent() : m_position(glm::vec2()) {}

	virtual bool init(void* desc) override final;

	GCC_MEMORYPOOL_DECLARATION(32);
};