#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>


using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void writeOnConsole(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(int argc, char** argv)
{
	glfwInit();

	// GLFW Window options
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Create GLFW window and make Context
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == nullptr)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	// Load OpenGL functions using GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
	}

	// Define viewport of OpenGL in GLFW window
	glViewport(0, 0, 800, 600);

	// When window (GLFW) will be resized, the viewport (OpenGL) will be adapted
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Callback function to catch input keys
	glfwSetKeyCallback(window, writeOnConsole);


	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		

		glfwSwapBuffers(window); // load back buffer (set of pixel) to front buffer (one rendered)
		glfwPollEvents(); // Check input events
	}

	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void writeOnConsole(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Close window if ESCAPE was clicked
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, true);

	// Print keys in the console
	const char* kk = glfwGetKeyName(key, scancode);
	if (kk != nullptr)
	{
		while (*kk != '\0')
			cout << *(kk++);
	}

	if (key == GLFW_KEY_C) 
	{
		glClearColor(0.6f, 0.2f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}