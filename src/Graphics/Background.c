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

void BackgroundInit(Background* background, size_t initial)
{
	background->layers = NULL;
	tb_array_reserve(background->layers, initial);
}

void BackgroundDestroy(Background* background)
{
	for (size_t i = 0; i < tb_array_len(background->layers); ++i)
		ignisDeleteTexture2D(&background->layers[i].texture);

	tb_array_free(background->layers);
}

size_t BackgroundPushLayer(Background* background, const char* path, float x, float y, float w, float h, float parallax)
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

	tb_array_push(background->layers, layer);

	return background->layers != NULL;
}

void BackgroundUpdate(Background* background, float x, float deltatime)
{
	for (size_t i = 0; i < tb_array_len(background->layers); ++i)
		BackgroundLayerUpdate(&background->layers[i], x, deltatime);
}

void BackgroundRender(const Background* background, const float* mat_view_proj)
{
	BatchRenderer2DStart(mat_view_proj);

	for (size_t i = 0; i < tb_array_len(background->layers); ++i)
		BackgroundLayerRender(&background->layers[i]);

	BatchRenderer2DFlush();
}
