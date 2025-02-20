#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <lodepng/lodepng.h>


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Board.h"
#include "Shader.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

using namespace std;

int initGLFWWindow(const int width, const int height);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void writeOnConsole(GLFWwindow* window, int key, int scancode, int action, int mods);
int checkShaderCompilation(unsigned int& shader);

void initTriangle();
void initRectangle();
int initCloseTriangles();

int gettingStarted();
int execGameOfLife();
int shaderExercises();
int textureExercises();


GLFWwindow* window;
unsigned int VAO;
unsigned int shaderProgram;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cerr << "No args" << endl;
		return -1;
	}

	if (*(argv[1]) == 's')
	{
		return gettingStarted();
	}
	else if (*(argv[1]) == 'g')
	{
		return execGameOfLife();
	}

	//shaderExercises();
	textureExercises();

	return 0;
}

/*
*	Main functions
*/
int gettingStarted()
{
	if (initGLFWWindow(WIN_WIDTH, WIN_HEIGHT) == -1)
		return -1;


	unsigned int numVerts;
	//initTriangle();
	//initRectangle();
	numVerts = initCloseTriangles();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // directive to OpenGL to draw like wireframe

	// Render loop
	while (!glfwWindowShouldClose(window))
	{

		glUseProgram(shaderProgram); // activate created program
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, numVerts);
		// Draw a rectangle using elements (multiple shapes) in EBO
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);

		glfwSwapBuffers(window); // load back buffer (set of pixel) to front buffer (one rendered)
		glfwPollEvents(); // Check input events
	}

	glfwTerminate();
	return 0;
}

int shaderExercises()
{
	if (initGLFWWindow(WIN_WIDTH, WIN_HEIGHT) == -1)
		return -1;

	float vertices[] = {
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

	Shader shd("shaders\\exercise\\ex1_vshd.glsl", "shaders\\exercise\\ex1_fshd.glsl");
	//Shader shd("background_vshd.glsl", "background_fshd.glsl");

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Define pointers for vertex position and color 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	double cursorX;
	double cursorY;

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		shd.use();


		glfwGetCursorPos(window, &cursorX, &cursorY); // get coordinates of viewport from 0 to 600
		int centerLocation = glGetUniformLocation(shd.programID, "center");
		glUniform3f(centerLocation, (cursorX / WIN_WIDTH) * 2 - 1, (cursorY / WIN_HEIGHT) * 2 - 1, 0.0f);
		cout << cursorX / WIN_WIDTH << "   " << cursorY / WIN_HEIGHT << endl;

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

int execGameOfLife()
{
	if (initGLFWWindow(WIN_WIDTH, WIN_HEIGHT) == -1)
		return -1;

	Board board{ 5, 5 };

	//Shader shd1("background_vshd.glsl", "background_fshd.glsl");
	Shader shd1("grid_vshd.glsl", "grid_fshd.glsl");

	drawingInfo* background = board.getBackgroundVertices();
	drawingInfo* grid = board.getGridVertices();

	unsigned int golVAO[2];
	unsigned int golVBO[2];
	unsigned int golEBO[2];

	glGenVertexArrays(2, golVAO);
	glGenBuffers(2, golVBO);
	glGenBuffers(2, golEBO);

	// Background
	glBindVertexArray(golVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, golVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, &(background->vertices[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, golEBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, &(background->indices[0]), GL_STATIC_DRAW);

	// Grid
	/*glBindVertexArray(golVAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, golVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, pippo, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);*/

	float firstVerts[]{
		-1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.0f, 0.0f, 0.0f,
	};

	glBindVertexArray(golVAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, golVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 72, &(grid->vertices[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	while (!glfwWindowShouldClose(window))
	{
		shd1.use();

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shd1.programID, "GridColorInput");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 0.0f);

		glBindVertexArray(golVAO[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		/*glUseProgram(shd1.programID);
		glBindVertexArray(golVAO[1]);
		glDrawArrays(GL_LINE, 0, 6);*/

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, golVAO);
	glDeleteBuffers(2, golVBO);
	glDeleteProgram(shd1.programID);
	//glDeleteProgram(shader2Program);

	return 0;
}

int textureExercises()
{
	if (initGLFWWindow(WIN_WIDTH, WIN_HEIGHT) == -1)
		return -1;

	float vertices[] = {
		-0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		0.5f, 0.5f, 0.0f,	1.0f, 0.5f, 0.0f,	1.0f, 1.0f,
		-0.5f, -0.5f, 0.0f,	0.5f, 1.0f, 0.0f,	0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,	0.0f, 0.5f, 1.0f,	1.0f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		1, 2, 3
	};

	Shader shd("shaders\\exercise\\ex2_vshd.glsl", "shaders\\exercise\\ex2_fshd.glsl");

	unsigned int t_VBO;
	unsigned int t_VAO;
	unsigned int t_EBO;

	glGenVertexArrays(1, &t_VAO);
	glGenBuffers(1, &t_VBO);
	glGenBuffers(1, &t_EBO);

	glBindVertexArray(t_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, t_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, t_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float)*3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);


	/*
	*	Define textures
	*/
	float borderColor[] = {
		1.0f, 0.5f, 0.5f, 1.0f
	};

	/*
	*	Define first texture unit
	*/
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Mipmap Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // mipmap used for downscale, then OpenGL gives an error

	/*
	*	Load image for texture
	*/
	int h, w, nc;
	unsigned char* data = stbi_load("C:\\Users\\smero\\Documents\\C++\\exercises\\ex2_gameoflife\\GameOfLife\\img\\container.jpg", &w, &h, &nc, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D); // automatically generate mipmap
	stbi_image_free(data); // good practice to clean the memory of the image


	/*
	*	Define second texture unit
	*/
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Mipmap Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // mipmap used for downscale, then OpenGL gives an error

	/*
	*	Load image for texture
	*/
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("C:\\Users\\smero\\Documents\\C++\\exercises\\ex2_gameoflife\\GameOfLife\\img\\undertale.png", &w, &h, &nc, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); // Use GL_RGBA for png alpha channel
	glGenerateMipmap(GL_TEXTURE_2D); // automatically generate mipmap
	stbi_image_free(data); // good practice to clean the memory of the image


	// Assigne uniform variable for shaders
	shd.use();
	glUniform1i(glGetUniformLocation(shd.programID, "texture1"), 0);
	glUniform1i(glGetUniformLocation(shd.programID, "texture2"), 1);


	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		shd.use();
		glBindVertexArray(t_VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &t_VAO);
	glDeleteBuffers(1, &t_VBO);
	glDeleteBuffers(1, &t_EBO);

	glfwTerminate();

	return 0;
}

/*
*	Utils
*/
int initGLFWWindow(const int width, const int height)
{
	glfwInit();

	// GLFW Window options
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Create GLFW window and make Context
	window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
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
	glViewport(0, 0, width, height);

	// When window (GLFW) will be resized, the viewport (OpenGL) will be adapted
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Callback function to catch input keys
	glfwSetKeyCallback(window, writeOnConsole);

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

void initTriangle() {
	float vertices[] {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
	};

	/*
	*	1. Bind VAO (Vertex Array Object)
	*	2. Load vertex array data into VBO (Vertex Buffer Objects) 
	*/
	glGenVertexArrays(1, &VAO);

	unsigned int VBO; // Vertex Buffer Objects
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind buffer (VBO) to the buffer type target (the one for the vertex buffer objects)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // allocates memory and store user-defined data


	/*
	*	3.a Create and compile Vertex Shader 
	*/
	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // create shader in according to its type

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check result of compiling
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::COMPILER\n" << infoLog << endl;
	}

	/*
	*	3.b Create and compile Fragment Shader
	*/
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check result of compiling
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::FRAGMENT::COMPILER\n" << infoLog << endl;
	}


	/*
	*	4. Create Shader Program Object
	*	At the end of this step, the shaders will be load into the GPU
	*/
	//unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check result of compiling
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}

	// delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/*
	*	5. Link attributes of vertices
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);	

}

void initRectangle()
{
	/*
	*	0.a Define visible vertices
	*	0.b Define which vertices for each elementary shape (OpenGL think with triangle mainly)
	*	1.a Create VAO
	*	1.b Create VBO
	*	1.c Create Element Buffer Objects (EBO)
	*/
	float vertices[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/*
	*	2.a Create and compile Vertex Shader
	*/
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // create shader in according to its type

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check result of compiling
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::COMPILER\n" << infoLog << endl;
	}

	/*
	*	2.b Create and compile Fragment Shader
	*/
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check result of compiling
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::FRAGMENT::COMPILER\n" << infoLog << endl;
	}


	/*
	*	3. Create Shader Program Object
	*	At the end of this step, the shaders will be load into the GPU
	*/
	//unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check result of compiling
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}

	// delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/*
	*	5. Link attributes of vertices
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

int initCloseTriangles() {
	float firstVerts[]{
		-1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.0f, 0.0f, 0.0f,
	};

	float secondVerts[]{
		1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.0f, 0.0f, 0.0f,
	};

	const int size = (sizeof(firstVerts) / sizeof(float));
	float vertices[size * 2];
	copy(firstVerts, firstVerts + size, vertices);
	copy(secondVerts, secondVerts + size, vertices + size);

	/*
	*	1. Bind VAO (Vertex Array Object)
	*	2. Load vertex array data into VBO (Vertex Buffer Objects)
	*/
	glGenVertexArrays(1, &VAO);

	unsigned int VBO; // Vertex Buffer Objects
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind buffer (VBO) to the buffer type target (the one for the vertex buffer objects)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // allocates memory and store user-defined data


	/*
	*	3.a Create and compile Vertex Shader
	*/
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // create shader in according to its type

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check result of compiling
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::COMPILER\n" << infoLog << endl;
	}

	/*
	*	3.b Create and compile Fragment Shader
	*/
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check result of compiling
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::FRAGMENT::COMPILER\n" << infoLog << endl;
	}


	/*
	*	4. Create Shader Program Object
	*	At the end of this step, the shaders will be load into the GPU
	*/
	//unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check result of compiling
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}

	// delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/*
	*	5. Link attributes of vertices
	*	Define how VBO is structured
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return size * 2;
}

int checkShaderCompilation(unsigned int &shader)
{
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::COMPILER\n" << infoLog << endl;
		return -1;
	}

	return 0;
}