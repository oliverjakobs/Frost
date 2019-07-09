#include "EntityMananger.h"

#include "Script/JSONParser.h"

#include "Debugger.h"

Entity* EntityManager::CreateEntity(Scene* scene, const std::string& path)
{
	DEBUG_ASSERT(scene, "Scene is null");
	DEBUG_ASSERT(!path.empty(), "Path is emtpy");

	json root = jsonParseFile(path);

	// create Entity
	std::string name = jsonToString(root, "name");
	glm::vec2 position = jsonToVec2(root, "position");
	glm::vec2 dimension = jsonToVec2(root, "dimension"); 

	Entity* entity = new Entity(name, position, dimension);

	// ScriptComponent
	if (root.find("script") != root.end())
	{
		json script = root.at("script");

		std::string src = jsonToString(script, "src");

		entity->AddComponent(new ScriptComponent(scene->GetLua().BindLuaFunction("res/scripts/player.lua", "onUpdate")));
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
			
			entity->AddComponent(new PhysicsComponent(scene->GetMap()->CreateBody(position, halfDimension, type), bodyPos));
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
					constraint = scene->GetMap()->GetConstraint();
		}

		entity->AddComponent(new CameraComponent(constraint, cameraOffset));
	}

	// ImageComponent
	if (root.find("image") != root.end())
	{
		json image = root.at("image");

		std::string res = jsonToString(image, "resource");
		float width = jsonToFloat(image, "width");
		float height = jsonToFloat(image, "height");

		entity->AddComponent(new ImageComponent(new Image(ResourceManager::GetTextureAtlas(res), width, height)));
	}

	// AnimationComponent
	if (root.find("animation") != root.end())
	{
		json animation = root.at("animation");

		std::map<std::string, Animation> animations;
		std::string startAnim;

		for (auto&[name, anim] : animation.items())
		{
			int start = jsonToInt(anim, "start");
			int length = jsonToInt(anim, "length");
			float delay = jsonToFloat(anim, "delay");

			if (start == 0)
				startAnim = name;

			animations.insert(AnimationDef(name, Animation(start, length, delay)));
		}

		entity->AddComponent(new AnimationComponent(animations, startAnim));
	}

	return entity;
}
