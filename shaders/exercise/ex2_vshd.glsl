#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inCol;
layout (location = 2) in vec2 inTex;

out vec4 vertColors;
out vec2 texCoord;

void main()
{
	gl_Position = vec4(inPos, 1.0);
	vertColors = vec4(inCol, 1.0);
	texCoord = inTex;
}