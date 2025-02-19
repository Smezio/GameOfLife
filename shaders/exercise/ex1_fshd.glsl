#version 330 core
out vec4 FragColor;

in vec4 vertexColor;
in vec4 positionColor;

void main()
{
	//FragColor = vertexColor;
	FragColor = positionColor;
}