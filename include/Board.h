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
	Board(const unsigned int w, const unsigned int h, const unsigned int x = 10, const unsigned int y = 10);
	Board(Board&&) = delete;
	Board(const Board&) = delete;
		
	drawingInfo getGridVertices(bool normalized = false);
	drawingInfo getBackgroundVertices(bool normalized = false);
	unsigned int getAbscissa();
	unsigned int getOrdinate();
};

