#pragma once

#include <random>
#include "MCTS.h"

extern std::mt19937 gRNG;
extern std::uniform_real_distribution<float> gUniformDist;

void playVsHuman(int num_sims);

void alphaZero(int num_iters = -1, int num_eps = 100, float updateThreshold = 0.55f);