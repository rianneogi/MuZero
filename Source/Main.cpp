#include "Training.h"
#include <chrono>

int main()
{
	srand(time(0));
	gRNG = std::mt19937(std::chrono::steady_clock::now().time_since_epoch().count());
	gUniformDist = std::uniform_real_distribution<float>(0.0, 1.0);
	initCuda();

	playVsHuman(10000);
	// alphaZero();
	return 0;
}