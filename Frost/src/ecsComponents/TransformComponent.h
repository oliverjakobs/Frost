#pragma once

#include "ECS/ecs.h"
#include "Maths/Maths.h"

class TransformComponent : public ecsComponent
{
public:
	GENERATE_COMPONENT_TYPE_ID(TransformComponent)

	glm::vec2 m_position;

	TransformComponent() : m_position(glm::vec2()) {}

	virtual bool init(void* desc) override final;

	GCC_MEMORYPOOL_DECLARATION(32);
};