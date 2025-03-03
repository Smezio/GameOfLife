#include "Game.h"

Game::Game()
{
	state = GameState::GAME_SUSPENDED;
	winManager = nullptr;
	board = nullptr;
	updateInterval = 2.0f;
	generations = 0;
}

Game::Game(const unsigned int boardX, const unsigned int boardY, const unsigned int width, const unsigned int height, const string title)
{
	state = GameState::GAME_SUSPENDED;
	winManager = WindowManager::getInstance(width, height, title);
	board = new Board(boardX, boardY);
	updateInterval = 2.0f;
	generations = 0;
}

Game::~Game()
{
	delete board;
}

void Game::init(const unsigned int width, const unsigned int height)
{
	board->initLayout(width, height);

	vector<Meeple*> people = board->getMeepleInfo();
	for (int i = 0; i < board->getAbscissa() * board->getOrdinate(); ++i)
		people.at(i)->initMeeple(width, height);
}

void Game::processMouseInput(GLFWwindow* window, int button, int action, int mods)
{
	double posX, posY;
	glfwGetCursorPos(winManager->getWindow(), &posX, &posY);

	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
	{
		execPlayBtn(posX, posY);

		selectMeeple(posX, posY);

		cout << posX << "   " << posY << endl;
	}
}

void Game::update()
{
	if (state != GameState::GAME_STARTED)
		return;

	//update game objs
	int deltaTime = glfwGetTime() - lastUpdate;

	// Check neighbours
	if (chrono::seconds(deltaTime).count() > updateInterval)
	{
		lastUpdate = glfwGetTime();
		cout << "2 seconds" << endl;

		// update meeples' states
		vector<Meeple*> meeples = board->getMeepleInfo();
		int x = board->getAbscissa();
		int y = board->getOrdinate();
		
		for (int i=0; i<(x*y); ++i)
		{
			int alive = 0;

			if ((i - y) - 1 >= 0 && (i - y) - 1 < (x * y))
				alive = alive + (meeples[(i - y) - 1]->getState() == MeepleState::ALIVE ? 1 : 0);

			if ((i - y) >= 0 && (i - y) < (x * y))
				alive = alive + (meeples[i-y]->getState() == MeepleState::ALIVE ? 1 : 0);

			if ((i - y) + 1 >= 0 && (i - y) + 1 < (x * y))
				alive = alive + (meeples[(i - y) + 1]->getState() == MeepleState::ALIVE ? 1 : 0);

			if (i - 1 >= 0 && (i -1) < (x * y))
				alive = alive + (meeples[i - 1]->getState() == MeepleState::ALIVE ? 1 : 0);

			if (i + 1 >= 0 && (i + 1) < (x * y))
				alive = alive + (meeples[i + 1]->getState() == MeepleState::ALIVE ? 1 : 0);

			if ((i + y) - 1 >= 0 && (i + y) - 1 < (x * y))
				alive = alive + (meeples[(i + y) - 1]->getState() == MeepleState::ALIVE ? 1 : 0);

			if ((i + y) >= 0 && (i + y) < (x * y))
				alive = alive + (meeples[i + y]->getState() == MeepleState::ALIVE ? 1 : 0);

			if ((i + y) + 1 >= 0 && (i + y) + 1 < (x*y))
				alive = alive + (meeples[(i + y) + 1]->getState() == MeepleState::ALIVE ? 1 : 0);

			meeples[i]->checkNeighbours(alive);
		}

		for (auto var : meeples)
		{
			var->updateState();
		}

		++generations;
		cout << "Gen: " << generations << endl;
	}

}

void Game::render()
{
	//render all objects
	board->getBackgroundInfo().shader.use();
	glUniformMatrix4fv(glGetUniformLocation(board->getBackgroundInfo().shader.programID, "proj"), 1, GL_FALSE, &(winManager->getOrthoProj()[0][0]));
	glBindVertexArray(board->getBackgroundInfo().VAO);
	glDrawElements(GL_TRIANGLES, board->getBackgroundInfo().indices.size(), GL_UNSIGNED_INT, 0);

	board->getGridInfo().shader.use();
	glUniformMatrix4fv(glGetUniformLocation(board->getGridInfo().shader.programID, "proj"), 1, GL_FALSE, &(winManager->getOrthoProj()[0][0]));
	glBindVertexArray(board->getGridInfo().VAO);
	glDrawArrays(GL_LINES, 0, board->getGridInfo().vertices.size());

	board->getPlayBtnVertices().shader.use();
	glUniformMatrix4fv(glGetUniformLocation(board->getPlayBtnVertices().shader.programID, "proj"), 1, GL_FALSE, &(winManager->getOrthoProj()[0][0]));
	glBindVertexArray(board->getPlayBtnVertices().VAO);
	glDrawElements(GL_TRIANGLES, board->getPlayBtnVertices().indices.size(), GL_UNSIGNED_INT, 0);

	vector<Meeple*> people = board->getMeepleInfo();
	for (int i = 0; i < (board->getAbscissa() * board->getOrdinate()); ++i)
	{
		if (people.at(i)->getState() == MeepleState::ALIVE)
		{
			people.at(i)->getMeepleInfo().shader.use();
			glUniformMatrix4fv(glGetUniformLocation(people.at(i)->getMeepleInfo().shader.programID, "proj"), 1, GL_FALSE, &(winManager->getOrthoProj()[0][0]));
			glBindVertexArray(people.at(i)->getMeepleInfo().VAO);
			glDrawElements(GL_TRIANGLES, people.at(i)->getMeepleInfo().indices.size(), GL_UNSIGNED_INT, 0);
		}
	}
}


void Game::execPlayBtn(double cursX, double cursY)
{
	int w, h;
	glfwGetWindowSize(getWindow(), &w, &h);

	int xS, yS, xE, yE;
	xS = board->getPlayBtnVertices().worldPosition.x * w;
	yS = board->getPlayBtnVertices().worldPosition.y * h;
	xE = (board->getPlayBtnVertices().worldScale.x * w) + xS;
	yE = (board->getPlayBtnVertices().worldScale.y * h) + yS;

	if (xS <= cursX && cursX <= xE && yS <= cursY && cursY <= yE)
	{
		if (state == GameState::GAME_SUSPENDED)
			state = GameState::GAME_STARTED;
		else
			state = GameState::GAME_SUSPENDED;
	}

	cout << (int)state << endl;
}

void Game::selectMeeple(double cursX, double cursY)
{
	if (state != GameState::GAME_SUSPENDED)
		return;

	vector<Meeple*> meeples = board->getMeepleInfo();

	int w, h;
	glfwGetWindowSize(getWindow(), &w, &h);

	for(Meeple* var : meeples)
	{
		int xS, yS, xE, yE;
		xS = var->getMeepleInfo().worldPosition.x * w;
		yS = var->getMeepleInfo().worldPosition.y * h;
		xE = (var->getMeepleInfo().worldScale.x * w) + xS;
		yE = (var->getMeepleInfo().worldScale.y * h) + yS;

		if (xS <= cursX && cursX <= xE && yS <= cursY && cursY <= yE)
		{
			if (var->getState() == MeepleState::DEAD)
				var->setState(MeepleState::ALIVE);
			else
				var->setState(MeepleState::DEAD);
		}
	}
}