#include "Animator.h"

#include "Frost/Frost.h"

#include "toolbox/tb_json.h"

#include "Application/Debugger.h"

#include <stdlib.h>

void AnimatorStart(Animator* animator, Animation* animation)
{
	animator->frame = animation->start;
	animator->clock = 0.0f;
}

void AnimatorTick(Animator* animator, Animation* animation, float deltatime)
{
	animator->clock += deltatime;

	// change frame
	if (animator->clock > animation->delay)
	{
		animator->clock = 0.0f;
		animator->frame++;
	}

	// restart animation
	if (animator->frame >= animation->start + animation->length || animator->frame < animation->start)
		AnimatorStart(animator, animation);
}

void AnimatorFree(void* block)
{
	free(block);
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
			tb_json_string((char*)element.value, "{*", state_str, APPLICATION_STR_LEN, &i);

			EntityState state = EntityStateFromString(state_str);

			tb_json_element anim;
			tb_json_read_format((char*)element.value, &anim, "{'%s'", state_str);

			Animation animation;
			animation.start = tb_json_int((char*)anim.value, "{'start'", NULL, 0);
			animation.length = tb_json_int((char*)anim.value, "{'length'", NULL, 0);
			animation.delay = tb_json_float((char*)anim.value, "{'delay'", NULL, 0.0f);

			AnimatorAddAnimation(&animator, state, &animation);
		}
		EcsAddDataComponent(&scene->ecs, entity, COMPONENT_ANIMATOR, &animator);
	}
}
