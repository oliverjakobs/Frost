#include "ComponentList.h"

int EcsComponentListInit(EcsComponentList* list, size_t max_size)
{
	clib_array_alloc(&list->position, max_size, sizeof(EcsPositionComponent));
	clib_array_alloc(&list->physics, max_size, sizeof(EcsPhysicsComponent));
	clib_array_alloc(&list->movement, max_size, sizeof(EcsMovementComponent));
	clib_array_alloc(&list->texture, max_size, sizeof(EcsTextureComponent));
	clib_array_alloc(&list->animation, max_size, sizeof(EcsAnimationComponent));
	clib_array_alloc(&list->camera, max_size, sizeof(EcsCameraComponent));
	clib_array_alloc(&list->interaction, max_size, sizeof(EcsInteractionComponent));

	return 1;
}

void EcsComponentListDelete(EcsComponentList* list)
{
	EcsComponentListClear(list);

	clib_array_free(&list->position);
	clib_array_free(&list->physics);
	clib_array_free(&list->movement);
	clib_array_free(&list->texture);
	clib_array_free(&list->animation);
	clib_array_free(&list->camera);
	clib_array_free(&list->interaction);
}

void EcsComponentListClear(EcsComponentList* list)
{
	/* physics */
	for (size_t i = 0; i < list->physics.used; ++i)
		free(((EcsPhysicsComponent*)clib_array_get(&list->physics, i))->body);

	/* animataion */
	for (size_t i = 0; i < list->animation.used; ++i)
	{
		AnimatorDestroy(((EcsAnimationComponent*)clib_array_get(&list->animation, i))->animator);
		free(((EcsAnimationComponent*)clib_array_get(&list->animation, i))->animator);
	}

	clib_array_clear(&list->position);
	clib_array_clear(&list->physics);
	clib_array_clear(&list->movement);
	clib_array_clear(&list->texture);
	clib_array_clear(&list->animation);
	clib_array_clear(&list->camera);
	clib_array_clear(&list->interaction);
}

EcsPositionComponent* EcsComponentListGetNextPosition(EcsComponentList* list, float x, float y)
{
	EcsPositionComponent comp;
	comp.x = x;
	comp.y = y;
	
	return (EcsPositionComponent*)clib_array_push(&list->position, &comp);
}

EcsPhysicsComponent* EcsComponentListGetNextPhysics(EcsComponentList* list, Body* body, float x, float y)
{
	EcsPhysicsComponent comp;

	comp.body = body;
	comp.body_x = x;
	comp.body_y = y;

	return (EcsPhysicsComponent*)clib_array_push(&list->physics, &comp);
}

EcsMovementComponent* EcsComponentListGetNextMovement(EcsComponentList* list, float ms, float jp)
{
	EcsMovementComponent comp;

	comp.direction = DIRECTION_LEFT;
	comp.speed = ms;
	comp.jump_power = jp;

	return (EcsMovementComponent*)clib_array_push(&list->movement, &comp);
}

EcsTextureComponent* EcsComponentListGetNextTexture(EcsComponentList* list, IgnisTexture2D* texture, float width, float height, size_t frame)
{
	EcsTextureComponent comp;

	comp.texture = texture;
	comp.width = width;
	comp.height = height;
	comp.frame = frame;
	comp.render_flip = RENDER_FLIP_NONE;

	return (EcsTextureComponent*)clib_array_push(&list->texture, &comp);
}

EcsAnimationComponent* EcsComponentListGetNextAnimation(EcsComponentList* list, Animator* animator)
{
	EcsAnimationComponent comp;

	comp.animator = animator;

	return (EcsAnimationComponent*)clib_array_push(&list->animation, &comp);
}

EcsCameraComponent* EcsComponentListGetNextCamera(EcsComponentList* list, float smooth)
{
	EcsCameraComponent comp;

	comp.camera = NULL;
	comp.smooth = smooth;
	comp.scene_w = -1.0f;
	comp.scene_h = -1.0f;

	return (EcsCameraComponent*)clib_array_push(&list->camera, &comp);
}

EcsInteractionComponent* EcsComponentListGetNextInteraction(EcsComponentList* list, float radius)
{
	EcsInteractionComponent comp;

	comp.radius = radius;

	return (EcsInteractionComponent*)clib_array_push(&list->interaction, &comp);
}
