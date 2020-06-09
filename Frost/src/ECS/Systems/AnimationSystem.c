#include "AnimationSystem.h"

void EcsSystemAnimation(EcsEntity* entity, ComponentTable* components, float deltatime)
{
	EcsAnimationComponent* animation = (EcsAnimationComponent*)ComponentTableGetComponent(components, entity->name, COMPONENT_ANIMATION);
	EcsTextureComponent* texture = (EcsTextureComponent*)ComponentTableGetComponent(components, entity->name, COMPONENT_TEXTURE);

	if (!(animation && texture)) return;

	AnimatorTick(animation->animator, entity, deltatime);

	texture->frame = AnimatorGetFrame(animation->animator);
}
