#include "Global.h"
#include "Board.h"
#include "WindowManager.h"

#include <chrono>

enum class GameState {GAME_STARTED, GAME_SUSPENDED, GAME_TERMINED};

#pragma once
class Game
{
private:
	GameState state;
	Board* board;
	WindowManager* winManager;

	float updateInterval;
	int generations;
	float lastUpdate;

public:
	Game();
	Game(const unsigned int boardX, const unsigned int boardY, const unsigned int width, const unsigned int height, const string title);
	~Game();

	void init(const unsigned int width, const unsigned int height);

	// Game Loop
	void processMouseInput(GLFWwindow* window, int button, int action, int mods);
	void update();
	void render();

	void execPlayBtn(double cursX, double cursY);
	void selectMeeple(double cursX, double cursY);

	inline GLFWwindow* getWindow() { return winManager->getWindow(); };
	inline void setState(GameState newState) { state = newState; };
};

