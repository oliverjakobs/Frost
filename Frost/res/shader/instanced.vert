#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec2 aOffset;
layout (location = 3) in int aFrame;

out vec2 TexCoord;

uniform int uRows = 1;
uniform int uColumns = 1;

uniform int uRenderFlip = 0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec2 Flip(vec2 texCoord)
{
	if (uRenderFlip == 0)	// FLIP_NONE
		return texCoord;
	if (uRenderFlip == 1)	// FLIP_HORIZONTAL
		return abs(texCoord - vec2(1.0 / uColumns, 0.0));
}

vec2 GetFramePos(int frame)
{
	float x = (frame % uColumns) * (1.0f / uColumns);
	float y = 1 - (1.0f / uRows) - ((frame / uColumns) * (1.0f / uRows));

	return vec2(x, y);
}

void main()
{
	TexCoord = GetFramePos(aFrame) + Flip(aTexCoord);
    gl_Position = projection * view * model * vec4(aPos + aOffset, 0.0, 1.0);
}