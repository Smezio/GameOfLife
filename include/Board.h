#include <vector>

using namespace std;

struct drawingInfo {
	vector<float> vertices;
	vector<unsigned int> indices;
};

#pragma once
class Board
{
private:
	unsigned int x, y;
	drawingInfo background;
	drawingInfo grid;
	vector<vector<int>> dashboard;

public:
	Board(unsigned int x = 10, unsigned int y = 10);
	Board(Board&&) = delete;
	Board(const Board&) = delete;
		
	drawingInfo* getGridVertices();
	drawingInfo* getBackgroundVertices();
	unsigned int getAbscissa();
	unsigned int getOrdinate();
};

