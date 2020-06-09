#include "PlayerSystem.h"

#include "Application/Input.h"

void EcsSystemPlayer(EcsEntity* entity, ComponentTable* components, float deltatime)
{
	EcsPhysicsComponent* physics = (EcsPhysicsComponent*)ComponentTableGetComponent(components, entity->name, COMPONENT_PHYSICS);
	EcsTextureComponent* texture = (EcsTextureComponent*)ComponentTableGetComponent(components, entity->name, COMPONENT_TEXTURE);
	EcsMovementComponent* movement = (EcsMovementComponent*)ComponentTableGetComponent(components, entity->name, COMPONENT_MOVEMENT);
	EcsCameraComponent* camera = (EcsCameraComponent*)ComponentTableGetComponent(components, entity->name, COMPONENT_CAMERA);

	if (!(physics && texture && movement)) return;

	vec2 velocity;
	velocity.x = (-InputKeyPressed(KEY_A) + InputKeyPressed(KEY_D)) * movement->speed;
	velocity.y = physics->body->velocity.y;

	if (InputKeyPressed(KEY_SPACE) && physics->body->collidesBottom)
		velocity.y = movement->jump_power;
	else if (InputKeyReleased(KEY_SPACE) && (velocity.y > 0.0f))
		velocity.y *= 0.5f;

	if (velocity.x > 0.0f)
	{
		movement->direction = DIRECTION_RIGHT;
		texture->render_flip = RENDER_FLIP_NONE;
	}
	else if (velocity.x < 0.0f)
	{
		movement->direction = DIRECTION_LEFT;
		texture->render_flip = RENDER_FLIP_HORIZONTAL;
	}

	// apply velocity
	physics->body->velocity = velocity;

	// set view
	if (camera)
	{
		vec2 position = EcsEntityGetPosition(entity->name, components);

		float smooth_w = (camera->camera->size.x * 0.5f) * camera->smooth;
		float smooth_h = (camera->camera->size.y * 0.5f) * camera->smooth;

		float center_x = camera->camera->position.x;
		float center_y = camera->camera->position.y;

		if (position.x > camera->camera->position.x + smooth_w)
			center_x = position.x - smooth_w;
		if (position.x < camera->camera->position.x - smooth_w)
			center_x = position.x + smooth_w;

		if (position.y > camera->camera->position.y + smooth_h)
			center_y = position.y - smooth_h;
		if (position.y < camera->camera->position.y - smooth_h)
			center_y = position.y + smooth_h;

		// constraint
		float constraint_x = camera->camera->size.x * 0.5f;
		float constraint_y = camera->camera->size.y * 0.5f;

		if (center_x < constraint_x)
			center_x = constraint_x;
		if (center_x > camera->scene_w - constraint_x)
			center_x = camera->scene_w - constraint_x;

		if (center_y < constraint_y)
			center_y = constraint_y;
		if (center_y > camera->scene_h - constraint_y)
			center_y = camera->scene_h - constraint_y;

		camera->camera->position.x = center_x;
		camera->camera->position.y = center_y;
		CameraUpdateViewOrtho(camera->camera);
	}
}
