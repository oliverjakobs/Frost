#include "EntityMananger.h"

#include <tinyxml2.h>
#include <glm/glm.hpp>

#include "Log/Logger.h"

using namespace tinyxml2;

glm::vec2 GetVec2(XMLElement* element, const glm::vec2& default = glm::vec2())
{
	if (element != nullptr)
		return glm::vec2(element->FloatAttribute("x"), element->FloatAttribute("y"));

	return default;
}

int EntityManager::CreateEntity(Scene* scene, const char* xmlPath)
{
	if (scene == nullptr)
		return -1;

	XMLDocument doc;
	XMLError result = doc.LoadFile(xmlPath);

	if (result != XML_SUCCESS)
	{
		DEBUG_WARN("Failed to load file ({0}): {1}", xmlPath, result);
		return -1;
	}

	XMLElement* root = doc.FirstChildElement("Entity");

	if (root == nullptr)
	{
		DEBUG_WARN("Root not found ({0})", xmlPath);
		return -1;
	}

	// create Entity
	auto entity = scene->GetRegistry().create();

	// TransformComponent
	XMLElement* transform = root->FirstChildElement("TransformComponent");

	if (transform != nullptr)
	{
		scene->GetRegistry().assign<TransformComponent>(entity, GetVec2(transform->FirstChildElement("Position")), GetVec2(transform->FirstChildElement("Dimension")));
	}

	// MovementComponent
	XMLElement* move = root->FirstChildElement("MovementComponent");

	if (move != nullptr)
	{
		scene->GetRegistry().assign<MovementComponent>(entity, move->FloatAttribute("speed"), move->FloatAttribute("jump"));
	}

	return entity;
}
