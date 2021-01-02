#include "tile_renderer.h"

#include "Graphics/Primitives2D.h"

int TileRendererInit(TileRenderer* renderer, TileMap* map)
{
	size_t tile_count = map->height * map->width;

	ignisCreateQuadTextured(&renderer->quad, GL_STATIC_DRAW);

	vec2* offsets = malloc(sizeof(vec2) * tile_count);
	GLuint* frames = malloc(sizeof(GLuint) * tile_count);

	for (size_t i = 0; i < tile_count; ++i)
	{
		offsets[i].x = map->tiles[i].pos.x;
		offsets[i].y = map->tiles[i].pos.y;

		frames[i] = map->tiles[i].id;
	}

	ignisAddArrayBuffer(&renderer->quad.vao, sizeof(vec2) * tile_count, offsets, GL_DYNAMIC_DRAW);
	ignisVertexAttribFloat(2, 2, GL_FALSE, 2, 0);
	ignisVertexAttribDivisor(2, 1);

	ignisAddArrayBuffer(&renderer->quad.vao, sizeof(GLuint) * tile_count, frames, GL_DYNAMIC_DRAW);
	ignisVertexAttribInt(3, 1, 1, 0);
	ignisVertexAttribDivisor(3, 1);

	renderer->tile_count = tile_count;

	free(offsets);
	free(frames);

	ignisCreateShadervf(&renderer->shader, "res/shaders/tile.vert", "res/shaders/tile.frag");
	ignisUseShader(&renderer->shader);
	ignisSetUniform1i(&renderer->shader, "u_Texture", 0);
	ignisSetUniform1f(&renderer->shader, "u_TileSize", map->tile_size);

	renderer->uniform_location_view_proj = ignisGetUniformLocation(&renderer->shader, "u_ViewProjection");
	renderer->uniform_location_model = ignisGetUniformLocation(&renderer->shader, "u_Model");
	renderer->uniform_location_rows = ignisGetUniformLocation(&renderer->shader, "u_Rows");
	renderer->uniform_location_cols = ignisGetUniformLocation(&renderer->shader, "u_Columns");

    return 1;
}

void TileRendererDestroy(TileRenderer* renderer)
{
	ignisDeleteShader(&renderer->shader);
	ignisDeleteQuad(&renderer->quad);
}

void TileMapRender(TileRenderer* renderer, IgnisTexture2D* texture, mat4 view_proj)
{
	mat4 model = mat4_indentity();

	ignisUseShader(&renderer->shader);
	ignisSetUniformMat4l(&renderer->shader, renderer->uniform_location_view_proj, view_proj.v);
	ignisSetUniformMat4l(&renderer->shader, renderer->uniform_location_model, model.v);

	ignisSetUniform1il(&renderer->shader, renderer->uniform_location_rows, texture->rows);
	ignisSetUniform1il(&renderer->shader, renderer->uniform_location_cols, texture->columns);

	ignisBindQuad(&renderer->quad);
	ignisBindTexture2D(texture, 0);

	glDrawElementsInstanced(GL_TRIANGLES, renderer->quad.vao.element_count, GL_UNSIGNED_INT, NULL, (GLsizei)renderer->tile_count);
}

void TileMapRenderDebug(TileMap* map, mat4 view_proj)
{
	Primitives2DStart(view_proj.v);

	/* map border */
	Primitives2DRenderRect(0.0f, 0.0f, map->width * map->tile_size, map->height * map->tile_size, IGNIS_WHITE);


	for (size_t i = 0; i < (map->width * map->height); ++i)
	{
		Tile* tile = &map->tiles[i];
		switch (tile->type)
		{
		case TILE_SOLID:
			Primitives2DRenderRect(tile->pos.x, tile->pos.y, map->tile_size, map->tile_size, IGNIS_WHITE);
			break;
		default:
			break;
		}
		
	}

	Primitives2DFlush();
}
