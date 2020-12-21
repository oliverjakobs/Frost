#include "Animator.h"

#include "Frost/Frost.h"

#include "toolbox/tb_json.h"

#include "Application/Debugger.h"

#include <stdlib.h>

void AnimatorFree(void* block)
{
	free(block);
}

void AnimatorStart(Animator* animator, int start)
{
	animator->frame = start;
	animator->clock = 0.0f;
}

/* TODO: improve animation storage */
static int AnimatorAddAnimation(Animator* animator, EntityState state, Animation* animation)
{
	if (state >= NUM_ENTITY_STATES) return 0; 

	animator->animations[state] = *animation;

	if (animator->current == ENTITY_STATE_NULL)
		animator->current = state;

	return 1;
}

void AnimatorLoad(Scene* scene, EcsEntityID entity, vec2 pos, int z_index, char* json)
{
	tb_json_element element;
	tb_json_read(json, &element, "{'animation'");
	if (element.error == TB_JSON_OK)
	{
		Animator animator;

		animator.current = ENTITY_STATE_NULL;
		animator.clock = 0.0f;
		animator.frame = 0;
		memset(animator.animations, 0, NUM_ENTITY_STATES * sizeof(Animation));

		for (int i = 0; i < element.elements; i++)
		{
			char state_str[APPLICATION_STR_LEN];
			tb_json_string(element.value, "{*", state_str, APPLICATION_STR_LEN, &i);

			EntityState state = EntityStateFromString(state_str);

			tb_json_element anim;
			tb_json_read_format(element.value, &anim, "{'%s'", state_str);

			Animation animation;
			animation.start = tb_json_int(anim.value, "[0", NULL, 0);
			animation.length = tb_json_int(anim.value, "[1", NULL, 0);
			animation.delay = tb_json_float(anim.value, "[2", NULL, 0.0f);

			AnimatorAddAnimation(&animator, state, &animation);
		}
		EcsAddDataComponent(&scene->ecs, entity, COMPONENT_ANIMATOR, &animator);
	}
}
