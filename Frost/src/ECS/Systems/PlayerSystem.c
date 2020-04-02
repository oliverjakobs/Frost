#include "PlayerSystem.h"

#include "Application/Input.h"

void EcsSystemPlayer(EcsEntity* entity, float deltatime)
{
	if (!entity->physics) return;
	if (!entity->texture) return;
	if (!entity->movement) return;

	vec2 velocity;
	velocity.x = (-InputKeyPressed(KEY_A) + InputKeyPressed(KEY_D)) * entity->movement->speed;
	velocity.y = entity->physics->body->velocity.y;

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

	// apply velocity
	entity->physics->body->velocity = velocity;

	// set view
	if (entity->camera)
	{
		vec2 position = EcsEntityGetPosition(entity);

		float smooth_w = (entity->camera->camera->size.x * 0.5f) * entity->camera->smooth;
		float smooth_h = (entity->camera->camera->size.y * 0.5f) * entity->camera->smooth;

		float center_x = entity->camera->camera->position.x;
		float center_y = entity->camera->camera->position.y;

		if (position.x > entity->camera->camera->position.x + smooth_w)
			center_x = position.x - smooth_w;
		if (position.x < entity->camera->camera->position.x - smooth_w)
			center_x = position.x + smooth_w;

		if (position.y > entity->camera->camera->position.y + smooth_h)
			center_y = position.y - smooth_h;
		if (position.y < entity->camera->camera->position.y - smooth_h)
			center_y = position.y + smooth_h;

		// constraint
		float constraint_x = entity->camera->camera->size.x * 0.5f;
		float constraint_y = entity->camera->camera->size.y * 0.5f;

		if (center_x < constraint_x)
			center_x = constraint_x;
		if (center_x > entity->camera->scene_w - constraint_x)
			center_x = entity->camera->scene_w - constraint_x;

		if (center_y < constraint_y)
			center_y = constraint_y;
		if (center_y > entity->camera->scene_h - constraint_y)
			center_y = entity->camera->scene_h - constraint_y;

		entity->camera->camera->position.x = center_x;
		entity->camera->camera->position.y = center_y;
		CameraUpdateViewOrtho(entity->camera->camera);
	}
}
