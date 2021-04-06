#include "PlayerSystem.h"

#include "Frost/Frost.h"

void PlayerSystem(Ecs* ecs, const Scene* scene, float deltatime)
{
	EcsMap* map = EcsGetComponentMap(ecs, COMPONENT_PLAYER);
	for (EcsMapIter* iter = EcsMapIterator(map); iter; iter = EcsMapIterNext(map, iter))
	{
		Player* player = EcsMapIterValue(iter);
		ECS_REQUIRE_MAP(Movement, ecs, movement, iter, COMPONENT_MOVEMENT);
		ECS_REQUIRE_MAP(Sprite, ecs, sprite, iter, COMPONENT_SPRITE);
		ECS_REQUIRE_MAP(RigidBody, ecs, body, iter, COMPONENT_RIGID_BODY);
		ECS_OPTIONAL_MAP(CameraController, ecs, camera, iter, COMPONENT_CAMERA);

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
		CameraControllerMoveConstrained(camera, GetEntityCenter(ecs, EcsMapIterKey(iter)), 0.5f);
	}
}
