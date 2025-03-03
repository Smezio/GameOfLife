#include "WindowManager.h"

WindowManager::WindowManager(const unsigned int width, const unsigned int height, const string title, const float near, const float far) 
	: width{width}, height{height}, near{near}, far{far}
{
	glfwInit();

	// GLFW Window options
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);


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
	//glfwSetFramebufferSizeCallback(window, resizeWindowCallback);
}

WindowManager* WindowManager::getInstance(const unsigned int width, const unsigned int height, const string title, const float near, const float far)
{
	if (manager == nullptr)
		manager = new WindowManager(width, height, title, near, far);
	
	return manager;
}

GLFWwindow* WindowManager::getWindow()
{
	return window;
}

mat4 WindowManager::getOrthoProj()
{
	mat4 matrix{ 1.0f };
	return ortho(0.0f, (float)width, (float)height, 0.0f, near, far);
}