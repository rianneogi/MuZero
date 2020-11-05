// #include "Execution.h"
#pragma once

#include "NNInclude.h"

typedef int Action;

class Game 
{
public:
	int mTurn; //equal to 0 if player 1's turn and 1 if player 2's turn

	int mMaxActions;
	int mStateSize;

	Game() {};
	virtual ~Game() {};

	virtual bool isOver() const = 0;
	virtual void doAction(Action action) = 0;
	virtual void undoAction(Action action) = 0;
	// virtual byte_t* getState() = 0;
	// virtual int getStateSize() = 0;
	virtual void restart() = 0;
	virtual std::vector<Action> getActions() const = 0;  //returns the set of actions, they need not be legal
	virtual void stateToTensor(float *res) const = 0;
	virtual float* stateToTensor() const = 0;
	// virtual int getTurn() const = 0;
	virtual int getScore() const = 0;
	virtual bool isLegal(Action action) const = 0;
	virtual void print() const = 0;
};