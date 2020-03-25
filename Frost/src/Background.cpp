#include "Background.h"

#include "IgnisRenderer/IgnisRenderer.h"

int BackgroundInit(Background* background, size_t max_layer)
{
	background->layers = (BackgroundLayer*)malloc(sizeof(BackgroundLayer) * max_layer);
	background->layer_count = 0;
	background->max_layer = max_layer;

	return background->layers != NULL;
}

void BackgroundClear(Background* background)
{
	free(background->layers);
	background->layer_count = 0;
	background->max_layer = 0;
}

int BackgroundPushLayer(Background* background, IgnisTexture* texture, float x, float y, float w, float h, float parallax)
{
	if (background->layer_count >= background->max_layer)
		return 0;

	background->layers[background->layer_count].texture = texture;
	background->layers[background->layer_count].startpos = x;
	background->layers[background->layer_count].pos_x = x;
	background->layers[background->layer_count].pos_y = y;
	background->layers[background->layer_count].width = w;
	background->layers[background->layer_count].height = h;
	background->layers[background->layer_count].parallax = parallax;
	background->layer_count++;
	return 1;
}

static void _LayerUpdate(BackgroundLayer* layer, float x, float deltatime)
{
	float rel_dist = x * (1 - layer->parallax);
	float dist = x * layer->parallax;

	layer->pos_x = layer->startpos + dist;

	if (rel_dist > layer->startpos + layer->width) layer->startpos += layer->width;
	else if (rel_dist < layer->startpos - layer->width) layer->startpos -= layer->width;
}

static void _LayerRender(BackgroundLayer* layer)
{
	BatchRenderer2DRenderTexture(layer->texture, layer->pos_x - layer->width, layer->pos_y, layer->width, layer->height);
	BatchRenderer2DRenderTexture(layer->texture, layer->pos_x, layer->pos_y, layer->width, layer->height);
	BatchRenderer2DRenderTexture(layer->texture, layer->pos_x + layer->width, layer->pos_y, layer->width, layer->height);
}

void BackgroundUpdate(Background* background, float x, float deltatime)
{
	for (size_t i = 0; i < background->layer_count; i++)
		_LayerUpdate(&background->layers[i], x, deltatime);
}

void BackgroundRender(Background* background, const float* mat_view_proj)
{
	BatchRenderer2DStart(mat_view_proj);

	for (size_t i = 0; i < background->layer_count; i++)
		_LayerRender(&background->layers[i]);

	BatchRenderer2DFlush();
}
