#include "Board.h"
#include "WindowManager.h"
#include <math.h>

Board::Board(const unsigned int w, const unsigned int h, const unsigned int x, const unsigned int y) : x{ x }, y{ y }
{
	dashboard.resize(x, vector<int>(y, 0));

	/*background.vertices = {
		((float)x / 2)*-1, (float)y / 2, -2.0f,
		(float)x / 2, (float)y / 2, -2.0f,
		((float)x / 2) * -1, ((float)y / 2)*-1, -2.0f,
		(float)x / 2, ((float)y / 2) * -1, -2.0f
	};*/

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
	
	int maxSize = max(x, y);

	for (int i = 0; i <= maxSize; ++i)
	{
		if(i<=x)
		{
			// aggiungere altro capo della linea
			grid.vertices.push_back({ (float)i/x });
			grid.vertices.push_back({ 0.0f });
			grid.vertices.push_back({ 0.0f });
			grid.vertices.push_back({ (float)i/x });
			grid.vertices.push_back({ (float)y/y });
			grid.vertices.push_back({ 0.0f });
		}
		
		if (i <= y)
		{
			grid.vertices.push_back({ 0.0f });
			grid.vertices.push_back({ (float)i/y });
			grid.vertices.push_back({ 0.0f });
			grid.vertices.push_back({ (float)x/x });
			grid.vertices.push_back({ (float)i/y });
			grid.vertices.push_back({ 0.0f });
		}
	}	
};

drawingInfo Board::getGridVertices(bool normalized)
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

drawingInfo Board::getBackgroundVertices(bool normalized)
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