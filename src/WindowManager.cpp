#include "WindowManager.h"

WindowManager::WindowManager(const int width, const int height, string title)
{
	glfwInit();

	// GLFW Window options
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create GLFW window and make Context
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (window == nullptr)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	// Load OpenGL functions using GLADspo
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
	}

	// Define viewport of OpenGL in GLFW window
	glViewport(0, 0, width, height);

	
	// When window (GLFW) will be resized, the viewport (OpenGL) will be adapted
	glfwSetFramebufferSizeCallback(window, resizeWindowCallback);
}

WindowManager* WindowManager::getInstance(const int width, const int height, const string title)
{
	if (manager == nullptr)
		manager = new WindowManager(width, height, title);
	
	return manager;
}

GLFWwindow* WindowManager::getWindow()
{
	return window;
}

void WindowManager::resizeWindowCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}