#include "Renderer.h"

void GenerateIndices(GLuint* indices, size_t max, size_t step)
{
	GLuint offset = 0;
	for (size_t i = 0; i < max - step; i += step)
	{
		indices[i + 0] = offset + 0;
		indices[i + 1] = offset + 1;
		indices[i + 2] = offset + 2;

		indices[i + 3] = offset + 2;
		indices[i + 4] = offset + 3;
		indices[i + 5] = offset + 0;

		offset += 4;
	}
}

void GetTexture2DSrcRect(const IgnisTexture2D* texture, size_t frame, float* src_x, float* src_y, float* src_w, float* src_h)
{
	float w = 1.0f / texture->columns;
	float h = 1.0f / texture->rows;

	float x = (frame % texture->columns) * w;
	float y = 1 - h - ((frame / texture->columns) * h);

	*src_x = x;
	*src_y = y;
	*src_w = w;
	*src_h = h;
}
