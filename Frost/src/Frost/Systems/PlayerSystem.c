#include "PlayerSystem.h"

#include "Frost/FrostEcs.h"
#include "Application/Input.h"

void PlayerSystem(Ecs* ecs, float deltatime)
{
	EcsComponentMap* map = EcsGetComponentMap(ecs, COMPONENT_MOVEMENT);
	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		Movement* movement = EcsComponentMapIterValue(iter);

		Sprite* sprite = EcsGetDataComponent(ecs, EcsComponentMapIterKey(iter), COMPONENT_SPRITE);
		RigidBody* body = EcsGetDataComponent(ecs, EcsComponentMapIterKey(iter), COMPONENT_RIGID_BODY);
		if (!(body && sprite)) continue;

		CameraController* camera = EcsGetDataComponent(ecs, EcsComponentMapIterKey(iter), COMPONENT_CAMERA);

		vec2 velocity;
		velocity.x = (-InputKeyPressed(KEY_A) + InputKeyPressed(KEY_D)) * movement->speed;
		velocity.y = body->velocity.y;

		if (InputKeyPressed(KEY_SPACE) && body->collides_bottom)
			velocity.y = movement->jump_power;
		else if (InputKeyReleased(KEY_SPACE) && (velocity.y > 0.0f))
			velocity.y *= 0.5f;

		if (velocity.x > 0.0f)
		{
			movement->direction = MOVEMENT_RIGHT;
			sprite->flip = SPRITE_FLIP_NONE;
		}
		else if (velocity.x < 0.0f)
		{
			movement->direction = MOVEMENT_LEFT;
			sprite->flip = SPRITE_FLIP_HORIZONTAL;
		}

		// apply velocity
		body->velocity = velocity;

		// set view
		if (camera)
		{
			CameraMoveConstrained(camera, GetEntityPosition(ecs, EcsComponentMapIterKey(iter)), 0.5f);
			CameraUpdateViewOrtho(camera->camera);
		}
	}
}
