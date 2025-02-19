#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inCol;

out vec3 vertexColor;
out vec4 positionColor;

uniform vec3 center;

void main()
{
	gl_Position = vec4(-inPos.x + center.x, -inPos.y - center.y, -inPos.z, 1.0);
	vertexColor = inCol;
	positionColor = gl_Position;
}