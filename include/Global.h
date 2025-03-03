#ifndef DRAW_INFO
#define DRAW_INFO

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include "Shader.h"

using namespace std;
using namespace glm;

struct drawingInfo {
	vector<float> vertices;
	vector<unsigned int> indices;
	vec3 worldPosition;
	vec3 worldScale;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	Shader shader;

	drawingInfo();
};

#endif