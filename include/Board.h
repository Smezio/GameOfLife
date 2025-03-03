#include "Global.h"

#include "Shader.h"
#include "Meeple.h"

using namespace std;
using namespace glm;


#pragma once
class Board
{
private:
	unsigned int x, y;
	drawingInfo background;
	drawingInfo grid;
	drawingInfo playBtn;
	vector<Meeple*> meeples;

public:
	Board(const unsigned int x = 10, const unsigned int y = 10);
	~Board();
	Board(Board&&) = delete;
	Board(const Board&) = delete;
		
	drawingInfo getGridInfo(bool normalized = false);
	drawingInfo getBackgroundInfo(bool normalized = false);
	drawingInfo getPlayBtnVertices(bool normalized = false);
	vector<Meeple*> getMeepleInfo(bool normalized = false);
	unsigned int getAbscissa();
	unsigned int getOrdinate();

	void initLayout(const unsigned int width, const unsigned int height);
};

