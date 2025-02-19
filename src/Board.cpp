#include "Board.h"
#include <math.h>

Board::Board(const unsigned int x, const unsigned int y) : x{ x }, y{ y }
{
	dashboard.resize(x, vector<int>(y, 0));

	background.vertices = { 
		((float)x / 2)*-1, (float)y / 2, 0.0f,
		(float)x / 2, (float)y / 2, 0.0f,
		((float)x / 2) * -1, ((float)y / 2)*-1, 0.0f,
		(float)x / 2, ((float)y / 2) * -1, 0.0f
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
			grid.vertices.push_back({ (float)i });
			grid.vertices.push_back({ 0.0f });
			grid.vertices.push_back({ 0.0f });
			grid.vertices.push_back({ (float)i });
			grid.vertices.push_back({ (float)y });
			grid.vertices.push_back({ 0.0f });
		}
		
		if (i <= y)
		{
			grid.vertices.push_back({ 0.0f });
			grid.vertices.push_back({ (float)i });
			grid.vertices.push_back({ 0.0f });
			grid.vertices.push_back({ (float)x });
			grid.vertices.push_back({ (float)i });
			grid.vertices.push_back({ 0.0f });
		}
	}	
};

drawingInfo* Board::getGridVertices()
{
	return &grid;
}

drawingInfo* Board::getBackgroundVertices()
{

	return &background;
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