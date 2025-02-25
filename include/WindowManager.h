#include <string>
#include <iostream>

#include <glad/glad.h>
#include<GLFW/glfw3.h>

using namespace std;

#pragma once
class WindowManager
{
private:
	static WindowManager* manager; // class instance
	GLFWwindow* window;

	WindowManager(const int width, const int height, string title);

	static void resizeWindowCallback(GLFWwindow* window, int width, int height);

public:
	static WindowManager* getInstance(const int width = 800, const int height = 600, const string title = "<TITLE>");
	GLFWwindow* getWindow();
};

