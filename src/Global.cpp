#include "Global.h"

enum class MeepleState { ALIVE, DEAD, UNDEFINED };

drawingInfo::drawingInfo()
{
	vertices = vector<float>();
	indices = vector<unsigned int>();
	worldPosition = vec3(0.0f);
	worldScale = vec3(1.0f);
	VAO = 0;
	VBO = 0;
	EBO = 0;
	shader = Shader();
}