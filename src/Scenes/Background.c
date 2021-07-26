#include "Background.h"

#include "toolbox/tb_array.h"

BackgroundLayer* BackgroundInit(size_t initial)
{
    BackgroundLayer* background = NULL;
    if (initial > 0) tb_array_reserve(background, initial);
    return background;
}

void BackgroundDestroy(BackgroundLayer* bg)
{
    for (BackgroundLayer* it = bg; it != tb_array_last(bg); it++) ignisDeleteTexture2D(&it->texture);
    tb_array_free(bg);
}

BackgroundLayer* BackgroundPushLayer(BackgroundLayer* bg, IgnisTexture2D tex, float x, float y, float w, float h, float parallax)
{
    BackgroundLayer layer = (BackgroundLayer){ tex, x, x, y, w, h, parallax };
    tb_array_push(bg, layer);
    return bg;
}

void BackgroundUpdate(BackgroundLayer* bg, float x, float deltatime)
{
    for (BackgroundLayer* layer = bg; layer != tb_array_last(bg); layer++)
    {
        float rel_dist = x * (1 - layer->parallax);

        layer->x = layer->startpos + (x * layer->parallax);

        if (rel_dist > layer->startpos + layer->w)		layer->startpos += layer->w;
        else if (rel_dist < layer->startpos - layer->w) layer->startpos -= layer->w;
    }
}

void BackgroundRender(const BackgroundLayer* bg, const float* mat_view_proj)
{
    BatchRenderer2DStart(mat_view_proj);

    for (const BackgroundLayer* layer = bg; layer != tb_array_last(bg); layer++)
    {
        BatchRenderer2DRenderTexture(&layer->texture, layer->x - layer->w, layer->y, layer->w, layer->h);
        BatchRenderer2DRenderTexture(&layer->texture, layer->x, layer->y, layer->w, layer->h);
        BatchRenderer2DRenderTexture(&layer->texture, layer->x + layer->w, layer->y, layer->w, layer->h);
    }

    BatchRenderer2DFlush();
}
