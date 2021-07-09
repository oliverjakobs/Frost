#ifndef BACKGROUND_H
#define BACKGROUND_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Ignis/Ignis.h"

typedef struct
{
	IgnisTexture2D texture;

	float startpos;
	float pos_x;
	float pos_y;
	float width;
	float height;

	float parallax;
} BackgroundLayer;

typedef struct
{
	BackgroundLayer* layers;
} Background;

void BackgroundInit(Background* background, size_t initial);
void BackgroundDestroy(Background* background);

size_t BackgroundPushLayer(Background* background, const char* path, float x, float y, float w, float h, float parallax);

void BackgroundUpdate(Background* background, float x, float deltatime);
void BackgroundRender(const Background* background, const float* mat_view_proj);

#ifdef __cplusplus
}
#endif

#endif /* !BACKGROUND_H */