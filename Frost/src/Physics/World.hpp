#pragma once

#include "Body.hpp"

#include <vector>

struct World
{
	std::vector<Body*> bodies;

	glm::vec2 gravity;
};

void WorldLoad(World* world, const glm::vec2& gravity);
void WorldDestroy(World* world);

void WorldAddBody(World* world, Body* body);
void WorldRemoveBody(World* world, Body* body);

void WorldTick(World* world, float deltatime);

std::vector<Body*> WorldGetOtherBodies(World* world, const Body* body);