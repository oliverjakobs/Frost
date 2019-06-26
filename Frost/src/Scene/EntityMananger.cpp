#include "EntityMananger.h"

#include "Script/XMLParser.h"

#include "Script/JSONParser.h"
#include "File/FileReader.h"

#include "Log/Logger.h"

using json = nlohmann::json;

uint32_t EntityManager::CreateEntityXML(Scene* scene, const std::string& path)
{
	DEBUG_ASSERT(scene, "Scene is null");
	DEBUG_ASSERT(!path.empty(), "Path is emtpy");

	XMLParser xml(path.c_str());

	DEBUG_ASSERT(xml.HasRoot("Entity"), "Root not found ({0})", path)

	// create Entity
	auto entity = scene->GetRegistry().create();

	// TransformComponent
	XMLElement* transform = xml.GetRoot()->FirstChildElement("TransformComponent");

	if (transform != nullptr)
	{
		glm::vec2 position = XMLParser::to_vec2(transform->FirstChildElement("Position"));
		glm::vec2 dimension = XMLParser::to_vec2(transform->FirstChildElement("Dimension"));

		scene->GetRegistry().assign<TransformComponent>(entity, position, dimension);
	}

	// MovementComponent
	XMLElement* move = xml.GetRoot()->FirstChildElement("MovementComponent");

	if (move != nullptr)
	{
		float speed = move->FloatAttribute("speed");
		float jump = move->FloatAttribute("jump");

		scene->GetRegistry().assign<MovementComponent>(entity, speed, jump);
	}

	// PhysicsComponent
	XMLElement* phys = xml.GetRoot()->FirstChildElement("PhysicsComponent");

	if (phys != nullptr)
	{
		XMLElement* body = phys->FirstChildElement("Body");

		DEBUG_ASSERT(body, "PhysicsComponent is missing a body ({0})", path);

		glm::vec2 position = XMLParser::to_vec2(body->FirstChildElement("Position"));
		glm::vec2 halfDimension = XMLParser::to_vec2(body->FirstChildElement("HalfDimension"));
		BodyType type = FromString(body->Attribute("type"));

		glm::vec2 bodyPos = XMLParser::to_vec2(phys->FirstChildElement("BodyPosition"));

		scene->GetRegistry().assign<PhysicsComponent>(entity, scene->GetMap()->createBody(position, halfDimension, type), bodyPos);
	}

	// CameraComponent
	XMLElement* cam = xml.GetRoot()->FirstChildElement("CameraComponent");

	if (cam != nullptr)
	{
		Rect constraint;

		// TODO: x, y, w, h as attributes for constraint
		if (cam->FirstChildElement("Constraint")->Attribute("get", "map"))
			constraint = scene->GetMap()->getConstraint();

		glm::vec2 cameraOffset = XMLParser::to_vec2(cam->FirstChildElement("Offset"));

		scene->GetRegistry().assign<CameraComponent>(entity, constraint, cameraOffset);
	}

	// ImageComponent
	XMLElement* img = xml.GetRoot()->FirstChildElement("ImageComponent");

	if (img != nullptr)
	{
		std::string name = img->Attribute("name");

		scene->GetRegistry().assign<ImageComponent>(entity, ResourceManager::GetImage(name));
	}

	// AnimationComponent
	XMLElement* anim = xml.GetRoot()->FirstChildElement("AnimationComponent");

	if (anim != nullptr)
	{
		std::map<std::string, Animation> animations;

		for (XMLElement* elem = anim->FirstChildElement("Animation"); elem != nullptr; elem = elem->NextSiblingElement("Animation"))
		{
			std::string name = elem->Attribute("name");
			int start = elem->IntAttribute("start");
			int length = elem->IntAttribute("length");
			float delay = elem->FloatAttribute("delay");

			animations.insert(AnimationDef(name, Animation(start, length, delay)));
		}

		scene->GetRegistry().assign<AnimationComponent>(entity, animations);
	}

	return entity;
}

uint32_t EntityManager::CreateEntityJSON(Scene* scene, const std::string& path)
{
	DEBUG_ASSERT(scene, "Scene is null");
	DEBUG_ASSERT(!path.empty(), "Path is emtpy");

	json root = json::parse(ReadFile(path.c_str()));

	// create Entity
	auto entity = scene->GetRegistry().create();

	// TransformComponent
	if (root.find("transform") != root.end())
	{
		json transform = root.at("transform");

		glm::vec2 position = jsonToVec2(transform, "posX", "posY");
		glm::vec2 dimension = jsonToVec2(transform, "width", "height");

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

			glm::vec2 position = jsonToVec2(body, "posX", "posY");
			glm::vec2 halfDimension = jsonToVec2(body, "halfWidth", "halfHeight");
			BodyType type = FromString(jsonToString(body, "type"));

			glm::vec2 bodyPos = jsonToVec2(physics, "bodyPosX", "bodyPosY");
			
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
		glm::vec2 cameraOffset = jsonToVec2(camera, "offsetX", "offsetY");

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
