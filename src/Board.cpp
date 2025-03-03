#include "Board.h"
#include "WindowManager.h"
#include <math.h>

Board::Board(const unsigned int x, const unsigned int y) : x{ x }, y{ y }
{
	// Define background's vertices
	background.vertices = {
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, -0.0f
	};
	background.indices = {
		0, 1, 2,
		1, 2, 3
	};
	background.worldPosition = vec3(0.0f);
	background.worldScale = vec3(1.0f);

	glGenVertexArrays(1, &(background.VAO));
	glGenBuffers(1, &(background.VBO));
	glGenBuffers(1, &(background.EBO));

	background.shader = Shader("shaders/background_vshd.glsl", "shaders/background_fshd.glsl");
	
	// Define grid's vertices
	int maxSize = std::max(x, y);

	meeples = vector<Meeple*>();

	grid.worldPosition = vec3(0.0f);
	grid.worldScale = vec3(0.7f, 1.0f, 0.0f);
	for (int i = 0; i <= maxSize; ++i)
	{
		if(i<=x)
		{
			// head of line
			grid.vertices.push_back({ grid.worldScale.x * (float)i/x });
			grid.vertices.push_back({ 0.0f });
			grid.vertices.push_back({ 0.0f });
			// tail of line
			grid.vertices.push_back({ grid.worldScale.x * (float)i/x });
			grid.vertices.push_back({ grid.worldScale.y });
			grid.vertices.push_back({ 0.0f });
		}
		
		if (i <= y)
		{
			// head of line
			grid.vertices.push_back({ 0.0f });
			grid.vertices.push_back({ grid.worldScale.y * (float)i/y });
			grid.vertices.push_back({ 0.0f });
			// tail of line
			grid.vertices.push_back({ 0.7f });
			grid.vertices.push_back({ grid.worldScale.y * (float)i/y });
			grid.vertices.push_back({ 0.0f });
		}

		if (i < x)
		{
			// definire meeples position
			for (int j = 0; j < y; ++j)
			{
				meeples.push_back(new Meeple((y * i) + j,
					vec3(grid.worldScale.x * ((float)i / x + 0.005f), grid.worldScale.y * (((float)j / y + 0.005f)), 0.0f),
					vec3(0.8f * grid.worldScale.x / x, 0.8f * grid.worldScale.y / y, 0.0f)));
			}
		}
	}
	
	glGenVertexArrays(1, &(grid.VAO));
	glGenBuffers(1, &(grid.VBO));

	grid.shader = Shader("shaders/grid_vshd.glsl", "shaders/grid_fshd.glsl");

	// Define people vertices


	// Define play button's vertices
	playBtn.vertices = {
		0.0f, 0.25, 0.0f,
		1.0f, 0.25, 0.0f,
		0.0f, 0.75, 0.0f,
		1.0f, 0.75, 0.0f
	};
	playBtn.indices = {
		0, 1, 2,
		1, 2, 3
	};
	playBtn.worldPosition = vec3(0.75f, 0.2f, 0.0f);
	playBtn.worldScale = vec3(0.2f, 0.2f, 0.0f);

	glGenVertexArrays(1, &(playBtn.VAO));
	glGenBuffers(1, &(playBtn.VBO));
	glGenBuffers(1, &(playBtn.EBO));

	playBtn.shader = Shader("shaders/button_vshd.glsl", "shaders/button_fshd.glsl");
};

Board::~Board()
{
	glDeleteVertexArrays(1, &(background.VAO));
	glDeleteBuffers(1, &(background.VBO));
	glDeleteBuffers(1, &(background.EBO));

	glDeleteVertexArrays(1, &(grid.VAO));
	glDeleteBuffers(1, &(grid.VBO));

	glDeleteVertexArrays(1, &(playBtn.VAO));
	glDeleteBuffers(1, &(playBtn.VBO));
	glDeleteBuffers(1, &(playBtn.EBO));
}

drawingInfo Board::getGridInfo(bool normalized)
{
	if(!normalized)
		return grid;

	drawingInfo tmp = grid;
	for (int i = 0; i < tmp.vertices.size(); ++i)
	{
		if ((i + 1) % 3 == 1)
			tmp.vertices[i] /= x;
		if ((i + 1) % 3 == 2)
			tmp.vertices[i] /= y;
	}
	return tmp;
}

drawingInfo Board::getBackgroundInfo(bool normalized)
{
	if (!normalized)
		return background;

	drawingInfo tmp = background;
	for (int i = 0; i < tmp.vertices.size(); ++i)
	{
		if ((i + 1) % 3 == 1)
			tmp.vertices[i] /= x;
		if ((i + 1) % 3 == 2)
			tmp.vertices[i] /= y;
	}
	return tmp;
}

vector<Meeple*> Board::getMeepleInfo(bool normalized)
{
	if (!normalized);
		return meeples;
}

drawingInfo Board::getPlayBtnVertices(bool normalized)
{
	return playBtn;
}

/*
*	Return the abscissa
*/
unsigned int Board::getAbscissa()
{
	return x;
}

/*
*	Return the ordinate
*/
unsigned int Board::getOrdinate()
{
	return y;
}

void Board::initLayout(const unsigned int width, const unsigned int height)
{
	/* Background */
	glBindVertexArray(background.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, background.VBO);
	glBufferData(GL_ARRAY_BUFFER, background.vertices.size() * sizeof(float), background.vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, background.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, background.indices.size() * sizeof(unsigned int), background.indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);

	background.shader.use();
	mat4 bgM4 = mat4(1.0f);
	bgM4 = scale(bgM4, vec3(width * background.worldScale.x, height * background.worldScale.y, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(background.shader.programID, "model"), 1, GL_FALSE, &bgM4[0][0]);


	/* Grid */
	glBindVertexArray(grid.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, grid.VBO);
	glBufferData(GL_ARRAY_BUFFER, grid.vertices.size() * sizeof(float), grid.vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);

	//grid.shader = Shader("shaders/grid_vshd.glsl", "shaders/grid_fshd.glsl");
	grid.shader.use();
	mat4 gridM4 = mat4(1.0f);
	gridM4 = scale(gridM4, vec3(width, height, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(grid.shader.programID, "model"), 1, GL_FALSE, &gridM4[0][0]);


	/* Button */
	glBindVertexArray(playBtn.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, playBtn.VBO);
	glBufferData(GL_ARRAY_BUFFER, playBtn.vertices.size() * sizeof(float), playBtn.vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, playBtn.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, playBtn.indices.size() * sizeof(unsigned int), playBtn.indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);

	playBtn.shader.use();
	mat4 pbM4 = mat4(1.0f);
	pbM4 = translate(pbM4, vec3(width * playBtn.worldPosition.x, height * playBtn.worldPosition.y, 0.0f));
	pbM4 = scale(pbM4, vec3(width * playBtn.worldScale.x, height * playBtn.worldScale.y, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(playBtn.shader.programID, "model"), 1, GL_FALSE, &pbM4[0][0]);
}