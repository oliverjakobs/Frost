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
	float x, y;
	float w, h;

	float parallax;
} BackgroundLayer;

BackgroundLayer* BackgroundInit(size_t initial);
void BackgroundDestroy(BackgroundLayer* bg);

BackgroundLayer* BackgroundPushLayer(BackgroundLayer* bg, IgnisTexture2D tex, float x, float y, float w, float h, float parallax);

void BackgroundUpdate(BackgroundLayer* bg, float x, float deltatime);
void BackgroundRender(const BackgroundLayer* bg, const float* mat_view_proj);

#ifdef __cplusplus
}
#endif

#endif /* !BACKGROUND_H */