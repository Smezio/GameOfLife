#version 330 core
out vec4 GridColor;

uniform vec4 GridColorInput;

void main()
{
	GridColor = GridColorInput;
}