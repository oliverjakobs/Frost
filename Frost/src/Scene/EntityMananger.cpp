#include "EntityMananger.h"

#include "Script/JSONParser.h"

#include "Log/Logger.h"

unsigned int EntityManager::CreateEntity(Scene* scene, const std::string& path)
{
	DEBUG_ASSERT(scene, "Scene is null");
	DEBUG_ASSERT(!path.empty(), "Path is emtpy");

	json root = jsonParseFile(path);

	// create Entity
	auto entity = scene->GetRegistry().create();

	// TransformComponent
	if (root.find("transform") != root.end())
	{
		json transform = root.at("transform");

		glm::vec2 position = jsonToVec2(transform, "position");
		glm::vec2 dimension = jsonToVec2(transform, "dimension");

		scene->GetRegistry().assign<TransformComponent>(entity, position, dimension);
	}

	// MovementComponent
	if (root.find("movement") != root.end())
	{
		json movement = root.at("movement");

		float speed = jsonToFloat(movement, "speed");
		float jump = jsonToFloat(movement, "jump");

		scene->GetRegistry().assign<MovementComponent>(entity, speed, jump);
	}

	// PhysicsComponent
	if (root.find("physics") != root.end())
	{
		json physics = root.at("physics");

		if (physics.find("body") != physics.end())
		{
			json body = physics.at("body");

			glm::vec2 position = jsonToVec2(body, "position");
			glm::vec2 halfDimension = jsonToVec2(body, "halfDimension");
			BodyType type = FromString(jsonToString(body, "type"));

			glm::vec2 bodyPos = jsonToVec2(physics, "bodyPos");
			
			scene->GetRegistry().assign<PhysicsComponent>(entity, scene->GetMap()->createBody(position, halfDimension, type), bodyPos);
		}
		else
		{
			DEBUG_WARN("Could not assign PhysicsComponent: Body is missing ({0})", path);
		}
	}

	// CameraComponent
	if (root.find("camera") != root.end())
	{
		json camera = root.at("camera");

		Rect constraint;
		glm::vec2 cameraOffset = jsonToVec2(camera, "offset");

		// TODO: x, y, w, h as attributes for constraint
		if (camera.find("constraint") != camera.end())
		{
			json con = camera.at("constraint");

			if (con.find("get") != con.end())
				if (con.at("get") == "map")
					constraint = scene->GetMap()->getConstraint();
		}

		scene->GetRegistry().assign<CameraComponent>(entity, constraint, cameraOffset);
	}

	// ImageComponent
	if (root.find("image") != root.end())
	{
		json image = root.at("image");

		std::string res = jsonToString(image, "resource");

		scene->GetRegistry().assign<ImageComponent>(entity, ResourceManager::GetImage(res));
	}

	// AnimationComponent
	if (root.find("animation") != root.end())
	{
		json animation = root.at("animation");

		std::map<std::string, Animation> animations;

		for (auto&[name, anim] : animation.items())
		{
			int start = jsonToInt(anim, "start");
			int length = jsonToInt(anim, "length");
			float delay = jsonToFloat(anim, "delay");

			animations.insert(AnimationDef(name, Animation(start, length, delay)));
		}

		scene->GetRegistry().assign<AnimationComponent>(entity, animations);
	}

	return entity;
}
