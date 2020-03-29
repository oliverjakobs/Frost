#include "World.hpp"

#include <algorithm>

void WorldLoad(World* world, const glm::vec2& gravity)
{
	world->gravity = gravity;
	world->bodies = std::vector<Body*>();
}

void WorldDestroy(World* world)
{
}

void WorldAddBody(World* world, Body* body)
{
	body->world = world;
	world->bodies.push_back(body);
}

void WorldRemoveBody(World* world, Body* body)
{
	auto it = std::find_if(world->bodies.begin(), world->bodies.end(), [&](auto& e) { return e == body; });

	if (it != world->bodies.end())
	{
		(*it)->world = nullptr;
		world->bodies.erase(it);
	}
}

void WorldTick(World* world, float deltatime)
{
	for (auto& body : world->bodies)
	{
		if (body->type == BodyType::STATIC)
			continue;

		glm::vec2 oldPosition = body->position;

		BodyTick(body, deltatime);

		for (auto& other : WorldGetOtherBodies(world, body))
		{
			BodyResolveCollision(body, other, oldPosition);
		}
	}
}

std::vector<Body*> WorldGetOtherBodies(World* world, const Body* body)
{
	std::vector<Body*> others;

	for (auto& b : world->bodies)
	{
		if (b != body)
			others.push_back(b);
	}

	return others;
}
