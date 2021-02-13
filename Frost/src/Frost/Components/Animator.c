#include "Animator.h"

#include "Frost/FrostParser.h"
#include "Application/Logger.h"

#include <stdlib.h>

/* kept for when storage is improved */
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
	if (state >= NUM_ENTITY_STATES || state < 0) return 0; 

	animator->animations[state] = *animation;

	if (animator->current == ENTITY_STATE_NULL)
		animator->current = state;

	return 1;
}

void AnimatorLoad(char* json, Ecs* ecs, EcsEntityID entity)
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
			tb_json_element key;
			tb_json_read_param(element.value, &key, "{*", &i);

			EntityState state = FrostParseEntityState(key.value, key.bytelen);

			tb_json_element anim;
			tb_json_read_format(element.value, &anim, "{'%.*s'", key.bytelen, key.value);

			Animation animation;
			animation.start = tb_json_int(anim.value, "[0", NULL, 0);
			animation.length = tb_json_int(anim.value, "[1", NULL, 0);
			animation.delay = tb_json_float(anim.value, "[2", NULL, 0.0f);

			AnimatorAddAnimation(&animator, state, &animation);
		}
		EcsAddDataComponent(ecs, entity, COMPONENT_ANIMATOR, &animator);
	}
}
