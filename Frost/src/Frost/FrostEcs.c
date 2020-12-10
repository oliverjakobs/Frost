#include "FrostEcs.h"

#include <string.h>

void RegisterDataComponents(Ecs* ecs)
{
	EcsRegisterDataComponent(ecs, sizeof(Transform), NULL);
	EcsRegisterDataComponent(ecs, sizeof(RigidBody), NULL);
	EcsRegisterDataComponent(ecs, sizeof(Movement), NULL);
	EcsRegisterDataComponent(ecs, sizeof(Sprite), NULL);
	EcsRegisterDataComponent(ecs, sizeof(Animator), AnimatorFree);
	EcsRegisterDataComponent(ecs, sizeof(CameraController), NULL);
	EcsRegisterDataComponent(ecs, sizeof(Inventory), InventoryFree);
	EcsRegisterDataComponent(ecs, sizeof(Interaction), NULL);
	EcsRegisterDataComponent(ecs, sizeof(Interactor), NULL);
}

void RegisterOrderComponents(Ecs* ecs)
{
	EcsRegisterOrderComponent(ecs, sizeof(Template), NULL);
	EcsRegisterOrderComponent(ecs, sizeof(ZIndex), ZIndexCmp);
}

void AddUpdateSystems(Ecs* ecs)
{
	EcsAddUpdateSystem(ecs, PhysicsSystem);
	EcsAddUpdateSystem(ecs, PlayerSystem);
	EcsAddUpdateSystem(ecs, AnimationSystem);
	EcsAddUpdateSystem(ecs, InventoryUpdateSystem);
	EcsAddUpdateSystem(ecs, InteractionSystem);
}

void AddRenderSystems(Ecs* ecs)
{
	EcsAddRenderSystem(ecs, ECS_RENDER_STAGE_PRIMARY, RenderSystem);
	EcsAddRenderSystem(ecs, ECS_RENDER_STAGE_DEBUG, DebugRenderSystem);
	EcsAddRenderSystem(ecs, ECS_RENDER_STAGE_UI, InventoryRenderSystem);
}

/* ----------------------| Animations |------------------------------- */
void RegisterAnimationCondition()
{
	AnimationSystemAddCondition(AnimationConditionIdle);
	AnimationSystemAddCondition(AnimationConditionFall);
	AnimationSystemAddCondition(AnimationConditionWalk);
	AnimationSystemAddCondition(AnimationConditionJump);
}

const char* AnimationConditionToString(AnimationCondition value)
{
	if (value == CONDITION_IDLE) return "CONDITION_IDLE";
	if (value == CONDITION_FALL) return "CONDITION_FALL";
	if (value == CONDITION_WALK) return "CONDITION_WALK";
	if (value == CONDITION_JUMP) return "CONDITION_JUMP";
	return NULL;
}

AnimationCondition AnimationConditionFromString(const char* str)
{
	if (strcmp(str, "CONDITION_IDLE") == 0) return CONDITION_IDLE;
	if (strcmp(str, "CONDITION_FALL") == 0) return CONDITION_FALL;
	if (strcmp(str, "CONDITION_WALK") == 0) return CONDITION_WALK;
	if (strcmp(str, "CONDITION_JUMP") == 0) return CONDITION_JUMP;
	return 0;
}

/* ----------------------| Ecs utility |------------------------------ */
EcsEntityID GetEntityAt(Ecs* ecs, vec2 pos)
{
	EcsComponentMap* map = EcsGetComponentMap(ecs, COMPONENT_TRANSFORM);
	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		Transform* transform = EcsComponentMapIterValue(iter);

		vec2 min = vec2_sub(transform->position, (vec2) { transform->size.x / 2.0f, 0.0f });
		vec2 max = vec2_add(min, transform->size);

		if (vec2_inside(pos, min, max))
			return EcsComponentMapIterKey(iter);
	}

	return ECS_NULL_ENTITY;
}

void SetEntityPosition(Ecs* ecs, EcsEntityID entity, vec2 pos)
{
	Transform* transform = EcsGetDataComponent(ecs, entity, COMPONENT_TRANSFORM);
	if (transform) transform->position = pos;

	RigidBody* body = EcsGetDataComponent(ecs, entity, COMPONENT_RIGID_BODY);
	if (body) body->position = vec2_add(pos, body->offset);
}

vec2 GetEntityPosition(Ecs* ecs, EcsEntityID entity)
{
	Transform* transform = EcsGetDataComponent(ecs, entity, COMPONENT_TRANSFORM);
	if (transform) return transform->position;

	return vec2_zero();
}

vec2 GetEntityCenter(Ecs* ecs, EcsEntityID entity)
{
	Transform* transform = EcsGetDataComponent(ecs, entity, COMPONENT_TRANSFORM);

	if (transform)
	{
		vec2 center;
		center.x = transform->position.x;
		center.y = transform->position.y + (transform->size.y / 2.0f);

		return center;
	}

	return vec2_zero();
}

rect GetEntityRect(Ecs* ecs, EcsEntityID entity)
{
	Transform* transform = EcsGetDataComponent(ecs, entity, COMPONENT_TRANSFORM);

	rect r;
	r.min = vec2_zero();
	r.max = vec2_zero();

	if (transform)
	{
		r.min = vec2_sub(transform->position, (vec2) { transform->size.x / 2.0f, 0.0f });
		r.max = vec2_add(r.min, transform->size);
	}

	return r;
}