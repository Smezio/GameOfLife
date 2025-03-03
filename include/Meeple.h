#include "Global.h"

using namespace std;
using namespace glm;

#pragma once
enum class MeepleState { ALIVE, DEAD, UNDEFINED };

class Meeple
{
private:
	unsigned int id;
	MeepleState state;
	MeepleState nextState;
	drawingInfo meeple;
	static Shader shd;
	
public:
	Meeple();
	Meeple(unsigned int id, vec3 position, vec3 scale);
	~Meeple();

	void checkNeighbours(int alive);

	void initMeeple(const unsigned int width, const unsigned int height);

	inline MeepleState getState() { return state; };
	inline MeepleState getNextState() { return nextState; };
	inline void setState(MeepleState newState) { state = newState; };
	inline void setNextState(MeepleState newState) { state = newState; };
	inline void updateState() { state = nextState; nextState = MeepleState::UNDEFINED; };
	inline drawingInfo getMeepleInfo() { return meeple; };
};

