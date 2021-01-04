#include "PlayerSystem.h"

#include "Frost/FrostEcs.h"
#include "Application/Input.h"

void PlayerSystem(Ecs* ecs, float deltatime)
{
	EcsComponentMap* map = EcsGetComponentMap(ecs, COMPONENT_PLAYER);
	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		Player* player = EcsComponentMapIterValue(iter);
		ECS_COMPONENT_REQUIRE(Movement, ecs, movement, iter, COMPONENT_MOVEMENT);
		ECS_COMPONENT_REQUIRE(Sprite, ecs, sprite, iter, COMPONENT_SPRITE);
		ECS_COMPONENT_REQUIRE(RigidBody, ecs, body, iter, COMPONENT_RIGID_BODY);
		ECS_COMPONENT_OPTIONAL(CameraController, ecs, camera, iter, COMPONENT_CAMERA);

		vec2 velocity;
		velocity.x = (-InputKeyPressed(player->move_left) + InputKeyPressed(player->move_right)) * movement->speed;
		velocity.y = body->body.velocity.y;

		if (InputKeyPressed(player->jump) && body->body.collision_state[TILE_BOTTOM])
			velocity.y = movement->jump_power;
		else if (InputKeyReleased(player->jump) && (velocity.y > 0.0f))
			velocity.y *= 0.5f;

		if (InputKeyPressed(KEY_S))
			body->body.drop = 1;

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
		body->body.velocity = velocity;

		// set view
		CameraControllerMoveConstrained(camera, GetEntityPosition(ecs, EcsComponentMapIterKey(iter)), 0.5f);
	}
}
