#include "Animator.h"

#include "Frost/FrostParser.h"

#include <stdlib.h>

/* TODO: improve animation storage */
static int AnimatorAddAnimation(Animator* animator, EntityState state, Animation* animation)
{
    if (state >= NUM_ENTITY_STATES || state < 0) return 0; 

    animator->animations[state] = *animation;

    if (animator->state == ENTITY_STATE_NULL) animator->state = state;

    return 1;
}

void AnimatorLoad(char* ini, Scene* scene, EcsEntityID entity)
{
    if (!EcsGetDataComponent(&scene->ecs, entity, COMPONENT_SPRITE))
    {
        MINIMAL_ERROR("[ECS] Animator requires Sprite");
        return;
    }

    tb_ini_element element;
    tb_ini_group_next(ini, "animation", &element);
    if (element.error == TB_INI_OK)
    {
        Animator animator;

        animator.current = NULL;
        animator.state = ENTITY_STATE_NULL;
        animator.clock = 0.0f;
        animator.frame = 0;
        memset(animator.animations, 0, NUM_ENTITY_STATES * sizeof(Animation));

        while ((ini = tb_ini_group_next(ini, "animation", &element)) != NULL)
        {
            char name[APPLICATION_STR_LEN];
            size_t name_len = tb_ini_name(&element, name, APPLICATION_STR_LEN);

            EntityState state = FrostParseEntityState(name, name_len);

            Animation animation;
            animation.start = tb_ini_int(element.start, NULL, "start", 0);
            animation.length = tb_ini_int(element.start, NULL, "length", 0);
            animation.delay = tb_ini_float(element.start, NULL, "delay", 0.0f);

            AnimatorAddAnimation(&animator, state, &animation);
        }
        EcsAddDataComponent(&scene->ecs, entity, COMPONENT_ANIMATOR, &animator);
    }
}

/* kept for when storage is improved */
void AnimatorRelease(Animator* block)
{
    // free(block);
}

void AnimatorStart(Animator* animator, Animation* animation)
{
    animator->current = animation;
    animator->frame = animation->start;
    animator->clock = 0.0f;
}