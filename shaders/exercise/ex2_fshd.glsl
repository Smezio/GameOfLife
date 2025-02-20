#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec4 vertColors;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() 
{
	FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.4);
	//FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}