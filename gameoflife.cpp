#include "Global.h"
#include "WindowManager.h"
#include "Camera.h"
#include "Board.h"
#include "Shader.h"
#include "Game.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <functional>

using namespace std;
using namespace glm;


WindowManager* WindowManager::manager = nullptr;
Camera cam{ 0.0f, 0.0f, 20.0f };
Game game = Game(50, 50, 1366, 768, "Game of Life");

void useSeparatedMatrices(Shader& boardShd, int& w, int& h);
void moveCamera(GLFWwindow* window, int key, int scancode, int action, int mods);
void getMouseClick(GLFWwindow* window, int button, int action, int mods);

int main(int argc, char** argv)
{
	game.init(1366, 768);

	glfwSetMouseButtonCallback(game.getWindow(), getMouseClick);
	

	while (!glfwWindowShouldClose(game.getWindow()))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game.update();

		game.render();

		glfwSwapBuffers(game.getWindow());
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

int old_main(int argc, char** argv)
{
	WindowManager* winManager = WindowManager::getInstance(1366, 768, "Game of Life");
	glfwSetKeyCallback(winManager->getWindow(), moveCamera);
	int w, h;
	glfwGetWindowSize(winManager->getWindow(), &w, &h);

	Board board{(unsigned int)w, (unsigned int)h};
	Shader boardShd{ "shaders/background_vshd.glsl", "shaders/background_fshd.glsl" };


	float vertices[] = {
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	

	unsigned int VAO[3], VBO[3], EBO[3];
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(2, EBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, board.getBackgroundInfo().vertices.size() * sizeof(float), board.getBackgroundInfo().vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, board.getBackgroundInfo().indices.size() * sizeof(float), board.getBackgroundInfo().indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);

	boardShd.use();
	mat4 bgM4 = mat4{ 1.0f };
	//bgM4 = translate(bgM4, vec3(0.5f * (float)w, 0.5f * (float)h, 0.0f));
	bgM4 = scale(bgM4, vec3(w, h, 1.0f));
	bgM4 = translate(bgM4, vec3(0.0f, 0.0f, -2.0f));
	bgM4 = ortho(0.0f, (float)w, (float)h, 0.0f, -10.0f, 10.0f) * bgM4;
	glUniformMatrix4fv(glGetUniformLocation(boardShd.programID, "transMatrix"), 1, GL_FALSE, value_ptr(bgM4));




	Shader gridShd{ "shaders/grid_vshd.glsl", "shaders/grid_fshd.glsl" };

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, board.getGridInfo().vertices.size() * sizeof(float), board.getGridInfo().vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);


	gridShd.use();
	// Compute transformation matrix
	mat4 gridM4 = mat4(1.0f);	
	gridM4 = scale(gridM4, vec3(w, h, 1.0f));
	gridM4 = ortho(0.0f, (float)w, (float)h, 0.0f, -10.0f, 10.0f) * gridM4;
	glUniformMatrix4fv(glGetUniformLocation(gridShd.programID, "transMatrix"), 1, GL_FALSE, value_ptr(gridM4));


	while (!glfwWindowShouldClose(winManager->getWindow()))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		boardShd.use();
		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES, board.getBackgroundInfo().indices.size(), GL_UNSIGNED_INT, 0);


		gridShd.use();
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_LINES, 0, board.getGridInfo().vertices.size());

		glfwSwapBuffers(winManager->getWindow());
		glfwPollEvents();
	}

	glDeleteVertexArrays(3, VAO);
	glDeleteBuffers(3, VBO);
	glDeleteBuffers(3, EBO);

	glfwTerminate();

	return 0;

}


void useSeparatedMatrices(Shader& boardShd, int& w, int& h)
{
	// Model matrix to apply transformation to all objects' vertices to the global world space
	mat4 modelM = mat4(1.0f);
	//modelM = rotate(modelM, radians(5.0f), vec3(1.0f, 0.0f, 0.0f));

	// View matrix to move the "camera" backward. Apply transformation to all object forward
	mat4 viewM = mat4(1.0f);
	viewM = translate(viewM, vec3(0.0f, 0.0f, -10.0f));

	// Projection matrix to apply perspective projection to view
	mat4 projM = mat4(1.0f);
	projM = perspective(radians(45.0f), (float)w / (float)h, 0.1f, 100.0f);
	//projM = ortho(0.0f, (float)w, 0.0f, (float)h, 0.1f, 20.0f);

	boardShd.use();
	glUniformMatrix4fv(glGetUniformLocation(boardShd.programID, "model"), 1, GL_FALSE, value_ptr(modelM));
	glUniformMatrix4fv(glGetUniformLocation(boardShd.programID, "view"), 1, GL_FALSE, value_ptr(viewM));
	glUniformMatrix4fv(glGetUniformLocation(boardShd.programID, "proj"), 1, GL_FALSE, value_ptr(projM));
}



void moveCamera(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.position += cam.forward() * cam.speed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.position -= cam.forward() * cam.speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.position += cam.right() * cam.speed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.position -= cam.right() * cam.speed;
}

void getMouseClick(GLFWwindow* window, int button, int action, int mods)
{
	game.processMouseInput(window, button, action, mods);
}