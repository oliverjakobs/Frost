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
        velocity.x = (-minimalKeyDown(player->move_left) + minimalKeyDown(player->move_right)) * movement->speed;
        velocity.y = body->body.velocity.y;

        if (minimalKeyDown(player->jump) && body->body.collision_state[TILE_BOTTOM])
            velocity.y = movement->bounce;
        else if (!minimalKeyDown(player->jump) && (velocity.y > 0.0f))
            velocity.y *= 0.5f;

        if (minimalKeyPressed(GLFW_KEY_S))
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

        /* update entity state */
        if (body->body.velocity.y > 0.0f)
            EntitySetState(ecs, EcsMapIterKey(iter), ENTITY_STATE_JUMP);
        else if (!body->body.collision_state[TILE_BOTTOM] && (body->body.velocity.y <= 0.0f))
            EntitySetState(ecs, EcsMapIterKey(iter), ENTITY_STATE_FALL);
        else if (body->body.collision_state[TILE_BOTTOM] && (body->body.velocity.x != 0.0f))
            EntitySetState(ecs, EcsMapIterKey(iter), ENTITY_STATE_WALK);
        else if (body->body.collision_state[TILE_BOTTOM] && (body->body.velocity.x == 0.0f))
            EntitySetState(ecs, EcsMapIterKey(iter), ENTITY_STATE_IDLE);
        else
            EntitySetState(ecs, EcsMapIterKey(iter), ENTITY_STATE_JUMP);

        // set view
        CameraControllerMoveConstrained(camera, GetEntityCenter(ecs, EcsMapIterKey(iter)), 0.5f);
    }
}
