#version 330 core

in vec2 texCoords;

out vec4 shdCol;

uniform sampler2D texture1;

void main()
{
	shdCol = texture(texture1, texCoords);
	//shdCol = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}