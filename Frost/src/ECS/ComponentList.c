#include "ComponentList.h"

int EcsComponentListInit(EcsComponentList* list, size_t max_size)
{
	/* position */
	list->position = (EcsPositionComponent*)malloc(max_size * sizeof(EcsPositionComponent));
	list->position_used = 0;
	list->position_capacity = max_size;

	/* physics */
	list->physics = (EcsPhysicsComponent*)malloc(max_size * sizeof(EcsPhysicsComponent));
	list->physics_used = 0;
	list->physics_capacity = max_size;

	/* movement */
	list->movement = (EcsMovementComponent*)malloc(max_size * sizeof(EcsMovementComponent));
	list->movement_used = 0;
	list->movement_capacity = max_size;

	/* texture */
	list->texture = (EcsTextureComponent*)malloc(max_size * sizeof(EcsTextureComponent));
	list->texture_used = 0;
	list->texture_capacity = max_size;

	/* animataion */
	list->animation = (EcsAnimationComponent*)malloc(max_size * sizeof(EcsAnimationComponent));
	list->animation_used = 0;
	list->animation_capacity = max_size;

	/* camera */
	list->camera = (EcsCameraComponent*)malloc(max_size * sizeof(EcsCameraComponent));
	list->camera_used = 0;
	list->camera_capacity = max_size;

	/* interaction */
	list->interaction = (EcsInteractionComponent*)malloc(max_size * sizeof(EcsInteractionComponent));
	list->interaction_used = 0;
	list->interaction_capacity = max_size;

	return 1;
}

void EcsComponentListDelete(EcsComponentList* list)
{
	/* position */
	free(list->position);
	list->position_used = 0;
	list->position_capacity = 0;

	/* physics */
	for (size_t i = 0; i < list->position_used; ++i)
		free(list->physics[i].body);
	free(list->physics);
	list->physics_used = 0;
	list->physics_capacity = 0;

	/* movement */
	free(list->movement);
	list->movement_used = 0;
	list->movement_capacity = 0;

	/* texture */
	free(list->texture);
	list->texture_used = 0;
	list->texture_capacity = 0;

	/* animataion */
	for (size_t i = 0; i < list->animation_used; ++i)
	{
		AnimatorDestroy(list->animation[i].animator);
		free(list->animation[i].animator);
	}
	free(list->animation);
	list->animation_used = 0;
	list->animation_capacity = 0;

	/* camera */
	free(list->camera);
	list->camera_used = 0;
	list->camera_capacity = 0;

	/* interaction */
	free(list->interaction);
	list->interaction_used = 0;
	list->interaction_capacity = 0;
}

EcsPositionComponent* EcsComponentListGetNextPosition(EcsComponentList* list, float x, float y)
{
	if (list->position_used >= list->position_capacity)
		return NULL;

	list->position[list->position_used].x = x;
	list->position[list->position_used].y = y;

	return &list->position[list->position_used++];
}

EcsPhysicsComponent* EcsComponentListGetNextPhysics(EcsComponentList* list, Body* body, float x, float y)
{
	if (list->physics_used >= list->physics_capacity)
		return NULL;

	list->physics[list->physics_used].body = body;
	list->physics[list->physics_used].body_x = x;
	list->physics[list->physics_used].body_y = y;

	return &list->physics[list->physics_used++];
}

EcsMovementComponent* EcsComponentListGetNextMovement(EcsComponentList* list, float ms, float jp)
{
	if (list->movement_used >= list->movement_capacity)
		return NULL;

	list->movement[list->movement_used].direction = DIRECTION_LEFT;
	list->movement[list->movement_used].speed = ms;
	list->movement[list->movement_used].jump_power = jp;

	return &list->movement[list->movement_used++];
}

EcsTextureComponent* EcsComponentListGetNextTexture(EcsComponentList* list, IgnisTexture2D* texture, float width, float height, size_t frame)
{
	if (list->texture_used >= list->texture_capacity)
		return NULL;

	list->texture[list->texture_used].texture = texture;
	list->texture[list->texture_used].width = width;
	list->texture[list->texture_used].height = height;
	list->texture[list->texture_used].frame = frame;
	list->texture[list->texture_used].render_flip = RENDER_FLIP_NONE;

	return &list->texture[list->texture_used++];
}

EcsAnimationComponent* EcsComponentListGetNextAnimation(EcsComponentList* list, Animator* animator)
{
	if (list->animation_used >= list->animation_capacity)
		return NULL;

	list->animation[list->animation_used].animator = animator;

	return &list->animation[list->animation_used++];
}

EcsCameraComponent* EcsComponentListGetNextCamera(EcsComponentList* list, float smooth)
{
	if (list->camera_used >= list->camera_capacity)
		return NULL;

	list->camera[list->camera_used].camera = NULL;
	list->camera[list->camera_used].smooth = smooth;
	list->camera[list->camera_used].scene_w = -1.0f;
	list->camera[list->camera_used].scene_h = -1.0f;

	return &list->camera[list->camera_used++];
}

EcsInteractionComponent* EcsComponentListGetNextInteraction(EcsComponentList* list, float radius)
{
	if (list->interaction_used >= list->interaction_capacity)
		return NULL;

	list->interaction[list->interaction_used].radius = radius;

	return &list->interaction[list->interaction_used++];
}
