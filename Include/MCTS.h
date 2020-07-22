#pragma once

#include "Net.h"


float search(Node* s, Game* game, const Net& nn);
void getActionProb(float* res, Game* game,const Net& nn, int num_sims, float temp);
