#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec2 aOffset;
layout (location = 3) in int aFrame;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int uRows = 1;
uniform int uColumns = 1;

void main()
{
	float x = (aFrame % uColumns) * (1.0f / uColumns);
	float y = 1 - (1.0f / uRows) - ((aFrame / uColumns) * (1.0f / uRows));

	TexCoord = aTexCoord +  vec2(x, y);
    gl_Position = projection * view * model * vec4(aPos + aOffset, 0.0, 1.0);
}