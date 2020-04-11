#include "AnimationSystem.h"

void EcsSystemAnimation(EcsEntity* entity, float deltatime)
{
	if (!entity->animation) return;
	if (!entity->texture) return;

	AnimatorTick(entity->animation->animator, entity, deltatime);

	entity->texture->frame = AnimatorGetFrame(entity->animation->animator);
}
