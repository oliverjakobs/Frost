#include "AnimationSystem.h"

#include "Frost/FrostEcs.h"

#include "Frost/AnimationLoader.h"

static tb_array animation_conditions;

int AnimationSystemInit()
{
	if (tb_array_alloc(&animation_conditions, 4, sizeof(AnimationConditionCallback), 0.0f) != TB_ARRAY_OK)
		return 0;

	RegisterAnimationCondition();

	return 1;
}

void AnimationSystemDestroy()
{
	tb_array_free(&animation_conditions);
}

int AnimationSystemAddCondition(int (*cond)(Ecs*, EcsEntityID, int))
{
	AnimationConditionCallback condition;
	condition.func = cond;

	return tb_array_push(&animation_conditions, &condition) != NULL;
}

AnimationConditionCallback* AnimationSystemGetCondition(size_t index)
{
	return tb_array_get(&animation_conditions, index);
}

static void PlayAnimation(Animator* animator, const char* name)
{
	if (strcmp(animator->current, name) == 0)
		return;

	Animation* animation = tb_hashmap_find(&animator->animations, name);
	if (animation)
	{
		AnimationStart(animation);
		animator->current = tb_hashmap_get_key_ptr(&animator->animations, name);
	}
}

void AnimationSystem(Ecs* ecs, float deltatime)
{
	EcsComponentMap* map = EcsGetComponentMap(ecs, COMPONENT_ANIMATOR);
	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		Animator* animator = EcsComponentMapIterValue(iter);

		Sprite* sprite = EcsGetDataComponent(ecs, EcsComponentMapIterKey(iter), COMPONENT_SPRITE);

		if (!sprite) continue;

		Animation* current = tb_hashmap_find(&animator->animations, animator->current);
		if (!current)
		{
			sprite->frame = 0;
			continue;
		}

		AnimationTick(current, deltatime);
		tb_hashmap* transitions = &current->transitions;
		for (tb_hashmap_iter* t_iter = tb_hashmap_iterator(transitions); t_iter; t_iter = tb_hashmap_iter_next(transitions, t_iter))
		{
			AnimationConditionCallback* cond = AnimationSystemGetCondition(AnimationConditionFromString(tb_hashmap_iter_get_key(t_iter)));
			if (cond && cond->func(ecs, EcsComponentMapIterKey(iter), 0))
				PlayAnimation(animator, tb_hashmap_iter_get_value(t_iter));
		}
		sprite->frame = current->frame;
	}
}

int AnimationConditionJump(Ecs* ecs, EcsEntityID entity, int s)
{
	RigidBody* body = EcsGetDataComponent(ecs, entity, COMPONENT_RIGID_BODY);
	return body ? (body->velocity.y > 0.0f) : 0;
}

int AnimationConditionFall(Ecs* ecs, EcsEntityID entity, int s)
{
	RigidBody* body = EcsGetDataComponent(ecs, entity, COMPONENT_RIGID_BODY);
	return body ? (!body->collides_bottom && (body->velocity.y <= 0.0f)) : 0;
}

int AnimationConditionWalk(Ecs* ecs, EcsEntityID entity, int s)
{
	RigidBody* body = EcsGetDataComponent(ecs, entity, COMPONENT_RIGID_BODY);
	return body ? (body->collides_bottom && (body->velocity.x != 0.0f)) : 0;
}

int AnimationConditionIdle(Ecs* ecs, EcsEntityID entity, int s)
{
	RigidBody* body = EcsGetDataComponent(ecs, entity, COMPONENT_RIGID_BODY);
	return body ? (body->collides_bottom && (body->velocity.x == 0.0f)) : 0;
}