#pragma once

#include <entt/entt.hpp>

#include "Graphics.h"
#include "Animation.h"
#include "Tile/Tilemap.h"
#include "Script/Lua/LuaBinding.h"

struct ImageComponent
{
	unique_ptr<Image> image;
	int frame;

	ImageComponent(Image* img) : image(img), frame(0) {}
};

struct TransformComponent
{
	glm::vec2 position;
	glm::vec2 dimension;

	Direction direction;

	TransformComponent(const glm::vec2& pos, const glm::vec2& dim) : position(pos), dimension(dim), direction(Direction::RIGHT) { }
};

struct ScriptComponent
{
	sol::function func;

	ScriptComponent(sol::function& func) : func(func) { }
};

struct PhysicsComponent
{
	Body* body;
	glm::vec2 bodyPos;

	PhysicsComponent(Body* b, const glm::vec2& bPos) : body(b), bodyPos(bPos) { }
};

struct AnimationComponent
{
	std::map<std::string, Animation> animations;
	std::string currentAnimation;

	AnimationComponent(std::map<std::string, Animation> anims, const std::string& startingAnim) : animations(anims), currentAnimation(startingAnim) {}
};

struct CameraComponent
{
	Rect constraint;
	glm::vec2 cameraOffset;

	CameraComponent(const Rect& con, const glm::vec2& camOff) : constraint(con), cameraOffset(camOff) {}
};