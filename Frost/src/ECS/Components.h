#pragma once

#include "Core/Graphics.h"

struct ImageComponent
{
	Image* image;
};

struct TransformComponent
{
	glm::vec2 position;
	glm::vec2 dimension;
};

struct VelocityComponent
{
	glm::vec2 velocity;
};
