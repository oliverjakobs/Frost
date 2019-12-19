#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
uniform mat4 u_Src;

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = (u_Src * vec4(a_TexCoord, 0.0, 1.0)).xy;
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}