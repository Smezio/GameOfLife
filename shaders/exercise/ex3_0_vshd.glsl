#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inCol;

out vec4 shdCol;

uniform mat4 transform;

void main() 
{
	gl_Position = transform * vec4(inPos, 1.0);
	shdCol = vec4(inCol, 1.0);
}