#include <string>
#include <iostream>

#include <glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

#pragma once
class WindowManager
{
private:
	static WindowManager* manager; // class instance

	GLFWwindow* window;

	// Info for orthographic projection
	unsigned int width;
	unsigned int height;
	float near;
	float far;

	WindowManager(const unsigned int width, const unsigned int height, const string title, const float near, const float far);

public:
	static WindowManager* getInstance(const unsigned int width = 800, const unsigned int height = 600, const string title = "<TITLE>", const float near = -10.0f, const float far = 10.0f);
	GLFWwindow* getWindow();

	mat4 getOrthoProj();
	
};

