#include "Training.h"

int main()
{
	srand(time(0));
	initCuda();

	playVsHuman(10000);
	// alphaZero();
	return 0;
}