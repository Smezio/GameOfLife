#include "Meeple.h"
#include <iostream>

Meeple::Meeple()
{
	id = 0;
	state = MeepleState::DEAD;
	nextState = MeepleState::UNDEFINED;
	meeple = drawingInfo();
}

Meeple::Meeple(unsigned int id, vec3 position, vec3 scale) : id {id}
{
	state = MeepleState::DEAD;
	nextState = MeepleState::UNDEFINED;

	meeple.vertices = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};

	meeple.indices = {
		0, 1, 2,
		1, 2, 3
	};

	meeple.worldPosition = position;
	meeple.worldScale = scale;

	glGenVertexArrays(1, &(meeple.VAO));
	glGenBuffers(1, &(meeple.VBO));
	glGenBuffers(1, &(meeple.EBO));

	meeple.shader = Shader( "shaders/meeple_vshd.glsl", "shaders/meeple_fshd.glsl" );
}

Meeple::~Meeple()
{
	glDeleteVertexArrays(1, &meeple.VAO);
	glDeleteBuffers(1, &meeple.VBO);
	glDeleteBuffers(1, &meeple.EBO);
}

void Meeple::checkNeighbours(int alive)
{
	if (state == MeepleState::ALIVE)
	{
		if (alive < 2 || alive > 3)
			nextState = MeepleState::DEAD;

		if (alive == 2 || alive == 3)
			nextState = MeepleState::ALIVE;
	}
	else
	{
		if (alive == 3)
			nextState = MeepleState::ALIVE;
		else
			nextState = MeepleState::DEAD;
	}
}

void Meeple::initMeeple(const unsigned int width, const unsigned int height)
{
	glBindVertexArray(meeple.VAO);

	glBindBuffer(GL_ARRAY_BUFFER, meeple.VBO);
	glBufferData(GL_ARRAY_BUFFER, meeple.vertices.size() * sizeof(float), meeple.vertices.data(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meeple.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meeple.indices.size() * sizeof(float), meeple.indices.data(), GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);

	meeple.shader.use();
	mat4 mpM4 = mat4(1.0f);
	mpM4 = translate(mpM4, vec3(width * meeple.worldPosition.x, height * meeple.worldPosition.y, meeple.worldPosition.z));
	mpM4 = scale(mpM4, vec3(width * meeple.worldScale.x, height * meeple.worldScale.y, meeple.worldScale.z));
	glUniformMatrix4fv(glGetUniformLocation(meeple.shader.programID, "model"), 1, GL_FALSE, &mpM4[0][0]);
}