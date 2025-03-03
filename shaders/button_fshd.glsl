#version 330 core
out vec4 GridColor;

in vec4 inCol;

void main()
{
	GridColor = inCol;
}