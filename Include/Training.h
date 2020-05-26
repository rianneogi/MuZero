#pragma once

#include "MCTS.h"

void playVsHuman(int num_sims);

void alphaZero(int num_iters = -1, int num_eps = 100, float updateThreshold = 0.55f);