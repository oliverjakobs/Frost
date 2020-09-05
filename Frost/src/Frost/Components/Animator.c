#include "Animator.h"

#include "Frost/FrostEcs.h"

#include "json/tb_json.h"

#include "Application/Debugger.h"

#include <stdlib.h>

void AnimatorFree(void* block)
{
	CLIB_HASHMAP_ITERATE_FOR(&((Animator*)block)->animations, iter)
	{
		free(clib_hashmap_iter_get_value(iter));
		clib_hashmap_iter_remove(&((Animator*)block)->animations, iter);
	}
	clib_hashmap_free(&((Animator*)block)->animations);
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

void AnimatorLoad(Scene* scene, EntityID entity, char* json)
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