#pragma once

#include "ECS/ecsComponent.h"

#include "Utility/Memory/MemoryMacros.h"

class TransformComponent : public ecsComponent
{
public:
	static constexpr ecsComponentID ID = 0;

	virtual bool init(void* desc) override final;

	GCC_MEMORYPOOL_DECLARATION(32);
};