#include "ScriptSystem.h"

void ScriptSystem::Tick(entt::registry& registry)
{
	auto view = registry.view<ScriptComponent>();

	for (auto entity : view)
	{
		view.get(entity).func(entity);
	}
};