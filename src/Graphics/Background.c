#include "Background.h"

#include "toolbox/tb_array.h"

#include "Graphics/Renderer.h"

static void BackgroundLayerUpdate(BackgroundLayer* layer, float x, float deltatime)
{
	float rel_dist = x * (1 - layer->parallax);

	layer->pos_x = layer->startpos + (x * layer->parallax);

	if (rel_dist > layer->startpos + layer->width) layer->startpos += layer->width;
	else if (rel_dist < layer->startpos - layer->width) layer->startpos -= layer->width;
}

static void BackgroundLayerRender(const BackgroundLayer* layer)
{
	BatchRenderer2DRenderTexture(&layer->texture, layer->pos_x - layer->width, layer->pos_y, layer->width, layer->height);
	BatchRenderer2DRenderTexture(&layer->texture, layer->pos_x, layer->pos_y, layer->width, layer->height);
	BatchRenderer2DRenderTexture(&layer->texture, layer->pos_x + layer->width, layer->pos_y, layer->width, layer->height);
}

BackgroundLayer* BackgroundInit(size_t initial)
{
	BackgroundLayer* background = NULL;
	if (initial > 0) tb_array_reserve(background, initial);
	return background;
}

void BackgroundDestroy(BackgroundLayer* background)
{
	for (size_t i = 0; i < tb_array_len(background); ++i)
		ignisDeleteTexture2D(&background[i].texture);

	tb_array_free(background);
}

size_t BackgroundPushLayer(BackgroundLayer** background, const char* path, float x, float y, float w, float h, float parallax)
{
	BackgroundLayer layer = (BackgroundLayer){
		.startpos = x,
		.pos_x = x,
		.pos_y = y,
		.width = w,
		.height = h,
		.parallax = parallax,
	};

	if (!ignisCreateTexture2D(&layer.texture, path, 1, 1, 1, NULL)) return 0;

	tb_array_push(*background, layer);

	return *background != NULL;
}

void BackgroundUpdate(BackgroundLayer* background, float x, float deltatime)
{
	for (size_t i = 0; i < tb_array_len(background); ++i)
		BackgroundLayerUpdate(&background[i], x, deltatime);
}

void BackgroundRender(const BackgroundLayer* background, const float* mat_view_proj)
{
	BatchRenderer2DStart(mat_view_proj);

	for (size_t i = 0; i < tb_array_len(background); ++i)
		BackgroundLayerRender(&background[i]);

	BatchRenderer2DFlush();
}
