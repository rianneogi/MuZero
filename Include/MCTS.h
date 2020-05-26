#pragma once

#include "Net.h"


float search(Node* s, TicTacToe& game, const Net& nn);
void getActionProb(float* res, TicTacToe& game,const Net& nn, int num_sims, float temp);
