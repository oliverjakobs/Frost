#include "PlayerSystem.h"

#include "Input/Input.h"

void EcsSystemPlayer(EcsEntity* entity, float deltatime)
{
	if (!entity->physics) return;
	if (!entity->texture) return;
	if (!entity->movement) return;

	vec2 velocity = vec2_(0.0f, entity->physics->body->velocity.y);

	if (InputKeyPressed(KEY_A))
		velocity.x -= entity->movement->speed;
	if (InputKeyPressed(KEY_D))
		velocity.x += entity->movement->speed;

	if (InputKeyPressed(KEY_SPACE) && entity->physics->body->collidesBottom)
		velocity.y = entity->movement->jump_power;
	else if (InputKeyReleased(KEY_SPACE) && (velocity.y > 0.0f))
		velocity.y *= 0.5f;

	if (velocity.x > 0.0f)
	{
		entity->movement->direction = DIRECTION_RIGHT;
		entity->texture->render_flip = RENDER_FLIP_NONE;
	}
	else if (velocity.x < 0.0f)
	{
		entity->movement->direction = DIRECTION_LEFT;
		entity->texture->render_flip = RENDER_FLIP_HORIZONTAL;
	}

	// Animation
	// if (velocity.y > 0.0f)
	// 	anim->PlayAnimation("jump");
	// else if (!collidesBottom)
	// 	anim->PlayAnimation("fall");
	// else if (velocity.x != 0.0f)
	// 	anim->PlayAnimation("walk");
	// else
	// 	anim->PlayAnimation("idle");

	// apply velocity
	entity->physics->body->velocity = velocity;

	// set view
	// SceneSetCameraPosition(scene, EntityGetPosition(m_entity));
}
