// #include "Execution.h"
#pragma once

#include "NNInclude.h"

typedef int Action;

class Game 
{
public:
	Game() {};
	virtual ~Game() {};

	virtual bool isOver() = 0;
	virtual int doAction(Action action) = 0;
	virtual void undoAction(Action action) = 0;
	// virtual byte_t* getState() = 0;
	// virtual int getStateSize() = 0;
	virtual void restart() = 0;
	virtual std::vector<Action> getActions() = 0;
	virtual void stateToTensor(float *res) = 0;
	virtual int getTurn() = 0;
	virtual int getScore() = 0;
	virtual bool isLegal(Action action) = 0;
	virtual void print() = 0;
};