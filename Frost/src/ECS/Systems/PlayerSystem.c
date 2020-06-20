#include "PlayerSystem.h"

#include "Application/Input.h"

void EcsSystemPlayer(ComponentTable* components, float deltatime)
{
	CLIB_DICT_ITERATE_FOR(&components->components[COMPONENT_MOVEMENT], iter)
	{
		EcsMovementComponent* movement = clib_dict_iter_get_value(iter);

		RigidBody* body = ComponentTableGetComponent(components, clib_dict_iter_get_key(iter), COMPONENT_RIGID_BODY);
		Sprite* sprite = ComponentTableGetComponent(components, clib_dict_iter_get_key(iter), COMPONENT_SPRITE);
		EcsCameraComponent* camera = ComponentTableGetComponent(components, clib_dict_iter_get_key(iter), COMPONENT_CAMERA);

		if (!(body && sprite)) continue;

		vec2 velocity;
		velocity.x = (-InputKeyPressed(KEY_A) + InputKeyPressed(KEY_D)) * movement->speed;
		velocity.y = body->velocity.y;

		if (InputKeyPressed(KEY_SPACE) && body->collides_bottom)
			velocity.y = movement->jump_power;
		else if (InputKeyReleased(KEY_SPACE) && (velocity.y > 0.0f))
			velocity.y *= 0.5f;

		if (velocity.x > 0.0f)
		{
			movement->direction = DIRECTION_RIGHT;
			sprite->flip = SPRITE_FLIP_NONE;
		}
		else if (velocity.x < 0.0f)
		{
			movement->direction = DIRECTION_LEFT;
			sprite->flip = SPRITE_FLIP_HORIZONTAL;
		}

		// apply velocity
		body->velocity = velocity;

		// set view
		if (camera)
		{
			vec2 position = EntityGetPosition(clib_dict_iter_get_key(iter), components);

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
}
