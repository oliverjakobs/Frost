#include "EntityMananger.h"

#include "Scene/Scene.h"

#include "Obelisk/Debugger.h"
#include "Script/JSONParser.h"

Entity* EntityManager::CreateEntity(Scene* scene, const std::string& path)
{
	if (path.empty())
	{
		OBELISK_WARN("[JSON] Could create Entity: path is empty");
		return nullptr;
	}

	json root = jsonParseFile(path);

	if (root.empty())
	{
		OBELISK_WARN("[JSON] Could create Entity: Failed to parse file ({0})", path);
		return nullptr;
	}

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

		if (!src.empty())
		{
			entity->AddComponent(new ScriptComponent(scene->GetLua(), src));
		}
		else
		{
			OBELISK_WARN("[JSON] Could not add ScriptComponent: Script src is missing ({0})", path);
		}
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
			OBELISK_WARN("[JSON] Could not add PhysicsComponent: Body is missing ({0})", path);
		}
	}

	// ImageComponent
	if (root.find("image") != root.end())
	{
		json image = root.at("image");

		std::string res = jsonToString(image, "resource");
		float width = jsonToFloat(image, "width");
		float height = jsonToFloat(image, "height");

		if (res.empty())
		{
			OBELISK_WARN("[JSON] Could not add ImageComponent: Resource is missing ({0})", path);
		}
		else if (width <= 0.0f || height <= 0.0f)
		{
			OBELISK_WARN("[JSON] Could not add ImageComponent: Widht or height is less than or equal to 0.0 ({0})", path);
		}
		else
		{
			entity->AddComponent(new TextureComponent(ResourceManager::GetTextureAtlas(res)));
		}
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

		if (!animations.empty())
		{
			entity->AddComponent(new AnimationComponent(animations));
		}
		else
		{
			OBELISK_WARN("[JSON] Could not add AnimationComponent: Animations missing ({0})", path);
		}
	}

	return entity;
}
