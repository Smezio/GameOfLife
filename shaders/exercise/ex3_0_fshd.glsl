#version 330 core

out vec4 outCol;

in vec4 shdCol;

void main()
{
	outCol = shdCol;
}