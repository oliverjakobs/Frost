#include "tile_renderer.h"


int TileRendererInit(TileRenderer* renderer, TileMap* map)
{
	size_t tile_count = map->height * map->width;

	float vertices[] =
	{
		0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f
	};

	IgnisBufferElement layout[] =
	{
		{GL_FLOAT, 2, GL_FALSE},	/* position */
		{GL_FLOAT, 2, GL_FALSE}		/* Texture coords*/
	};

	ignisGenerateVertexArray(&renderer->vao);
	ignisAddArrayBufferLayout(&renderer->vao, sizeof(vertices), vertices, GL_STATIC_DRAW, 0, layout, 2);

	vec2* offsets = malloc(sizeof(vec2) * tile_count);
	GLuint* frames = malloc(sizeof(GLuint) * tile_count);

	for (size_t i = 0; i < tile_count; ++i)
	{
		offsets[i].x = map->tiles[i].pos.x;
		offsets[i].y = map->tiles[i].pos.y;

		frames[i] = map->tiles[i].id;
	}

	ignisAddArrayBuffer(&renderer->vao, sizeof(vec2) * tile_count, offsets, GL_DYNAMIC_DRAW);
	ignisVertexAttribFloat(2, 2, GL_FALSE, 2, 0);
	ignisVertexAttribDivisor(2, 1);

	ignisAddArrayBuffer(&renderer->vao, sizeof(GLuint) * tile_count, frames, GL_DYNAMIC_DRAW);
	ignisVertexAttribInt(3, 1, 1, 0);
	ignisVertexAttribDivisor(3, 1);

	GLuint indices[] = { 0, 1, 2, 2, 3, 0 };
	ignisLoadElementBuffer(&renderer->vao, indices, 6, GL_STATIC_DRAW);

	renderer->tile_count = tile_count;

	free(offsets);
	free(frames);

	ignisCreateShadervf(&renderer->shader, "res/shaders/tile.vert", "res/shaders/tile.frag");
	ignisUseShader(&renderer->shader);
	ignisSetUniform1i(&renderer->shader, "u_Texture", 0);
	ignisSetUniform1f(&renderer->shader, "u_TileSize", map->tile_size);

    return 1;
}

void TileRendererDestroy(TileRenderer* renderer)
{
	ignisDeleteShader(&renderer->shader);
	ignisDeleteVertexArray(&renderer->vao);
}


void TileMapRender(TileRenderer* renderer, IgnisTexture2D* texture, vec2 offset, mat4 view_proj)
{
	mat4 model = mat4_indentity();

	ignisUseShader(&renderer->shader);
	ignisSetUniformMat4(&renderer->shader, "u_ViewProjection", view_proj.v);
	ignisSetUniformMat4(&renderer->shader, "u_Model", model.v);

	ignisSetUniform1i(&renderer->shader, "u_Rows", texture->rows);
	ignisSetUniform1i(&renderer->shader, "u_Columns", texture->columns);

	ignisBindVertexArray(&renderer->vao);
	ignisBindTexture2D(texture, 0);

	glDrawElementsInstanced(GL_TRIANGLES, renderer->vao.element_count, GL_UNSIGNED_INT, NULL, (GLsizei)renderer->tile_count);
}