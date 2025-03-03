#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 inCol;

uniform mat4 model;
uniform mat4 proj;

void main() 
{
	gl_Position = proj * model * vec4(aPos, 1.0);
	inCol = vec4(1.0, 0.0, 0.0, 1.0);
}