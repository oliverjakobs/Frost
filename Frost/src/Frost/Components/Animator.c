#include "Animator.h"

#include "Frost/FrostEcs.h"

#include "json/tb_json.h"

#include "Application/Debugger.h"

#include <stdlib.h>

/* ---------------------------------| Animation |----------------------------------- */
void AnimationLoad(Animation* animation, int start, int length, float delay, size_t initial)
{
	animation->start = start;
	animation->length = length;
	animation->delay = delay;
	animation->clock = 0.0f;
	animation->frame = 0;

	if (clib_hashmap_alloc(&animation->transitions, clib_hash_string, clib_hashmap_str_cmp, initial) == CLIB_HASHMAP_OK)
	{
		clib_hashmap_set_key_alloc_funcs(&animation->transitions, clib_hashmap_str_alloc, clib_hashmap_str_free);
		clib_hashmap_set_value_alloc_funcs(&animation->transitions, clib_hashmap_str_alloc, clib_hashmap_str_free);
	}
}

void AnimationDestroy(Animation* animation)
{
	clib_hashmap_free(&animation->transitions);
}

void AnimationStart(Animation* animation)
{
	animation->frame = animation->start;
	animation->clock = 0.0f;
}

void AnimationTick(Animation* animation, float deltatime)
{
	animation->clock += deltatime;

	// change frame
	if (animation->clock > animation->delay)
	{
		animation->clock = 0.0f;
		animation->frame++;
	}

	// restart animation
	if (animation->frame >= animation->start + animation->length || animation->frame < animation->start)
	{
		animation->frame = animation->start;
		animation->clock = 0.0f;
	}
}

/* ---------------------------------| AnimationCondition |-------------------------- */
static clib_hashmap condition_table; /* <str, AnimationCondition> */

void AnimationConditionsInit()
{
	if (clib_hashmap_alloc(&condition_table, clib_hash_string, clib_hashmap_str_cmp, 0) == CLIB_HASHMAP_OK)
		clib_hashmap_set_key_alloc_funcs(&condition_table, clib_hashmap_str_alloc, clib_hashmap_str_free);
}

void AnimationConditionsDestroy()
{
	for (clib_hashmap_iter* iter = clib_hashmap_iterator(&condition_table); iter; iter = clib_hashmap_iter_next(&condition_table, iter))
	{
		free(clib_hashmap_iter_get_value(iter));
		clib_hashmap_iter_remove(&condition_table, iter);
	}
	clib_hashmap_free(&condition_table);
}

int AnimationConditionsRegisterCondition(const char* name, int(*condition)(Ecs*, EcsEntityID, int))
{
	AnimationCondition* value = malloc(sizeof(AnimationCondition));

	if (!value) return 0;

	value->func = condition;
	if (clib_hashmap_insert(&condition_table, name, value) == value)
		return 1;

	free(value);
	return 0;
}

AnimationCondition* AnimationConditionsGetCondition(const char* name)
{
	return clib_hashmap_find(&condition_table, name);
}

/* ---------------------------------| Animator |------------------------------------ */
void AnimatorFree(void* block)
{
	clib_hashmap* map = &((Animator*)block)->animations;
	for (clib_hashmap_iter* iter = clib_hashmap_iterator(map); iter; iter = clib_hashmap_iter_next(map, iter))
	{
		free(clib_hashmap_iter_get_value(iter));
		clib_hashmap_iter_remove(map, iter);
	}
	clib_hashmap_free(map);
	free(block);
}

static int AnimatorAddAnimation(Animator* animator, const char* name, Animation* animation)
{
	if (clib_hashmap_insert(&animator->animations, name, animation) != animation)
	{
		AnimationDestroy(animation);
		free(animation);
		return 0;
	}

	if (animator->current == NULL)
		animator->current = clib_hashmap_get_key_ptr(&animator->animations, name);

	return 1;
}

void AnimatorLoad(Scene* scene, EcsEntityID entity, char* json)
{
	tb_json_element element;
	tb_json_read(json, &element, "{'animation'");
	if (element.error == TB_JSON_OK)
	{
		Animator animator;

		animator.current = NULL;

		if (clib_hashmap_alloc(&animator.animations, clib_hash_string, clib_hashmap_str_cmp, 0) != CLIB_HASHMAP_OK)
		{
			DEBUG_WARN("[Animator] Failed to allocate animation map");
			return;
		}

		clib_hashmap_set_key_alloc_funcs(&animator.animations, clib_hashmap_str_alloc, clib_hashmap_str_free);
		for (int i = 0; i < element.elements; i++)
		{
			char anim_name[APPLICATION_STR_LEN];
			tb_json_string((char*)element.value, "{*", anim_name, APPLICATION_STR_LEN, &i);

			tb_json_element anim;
			tb_json_read_format((char*)element.value, &anim, "{'%s'", anim_name);

			unsigned int start = tb_json_int((char*)anim.value, "{'start'", NULL, 0);
			unsigned int length = tb_json_int((char*)anim.value, "{'length'", NULL, 0);
			float delay = tb_json_float((char*)anim.value, "{'delay'", NULL, 0.0f);

			tb_json_element transition_array;
			tb_json_read((char*)anim.value, &transition_array, "{'transitions'");

			Animation* animation = (Animation*)malloc(sizeof(Animation));
			AnimationLoad(animation, start, length, delay, transition_array.elements);

			char* value = (char*)transition_array.value;
			tb_json_element transition_element;
			for (int i = 0; i < transition_array.elements; i++)
			{
				value = tb_json_array_step(value, &transition_element);

				if (transition_element.data_type == TB_JSON_ARRAY)
				{
					char condition[APPLICATION_STR_LEN];
					tb_json_string((char*)transition_element.value, "[0", condition, APPLICATION_STR_LEN, NULL);
					char next[APPLICATION_STR_LEN];
					tb_json_string((char*)transition_element.value, "[1", next, APPLICATION_STR_LEN, NULL);

					clib_hashmap_insert(&animation->transitions, condition, next);
				}
			}

			AnimatorAddAnimation(&animator, anim_name, animation);
		}
		EcsAddDataComponent(&scene->ecs, entity, COMPONENT_ANIMATION, &animator);
	}
}
