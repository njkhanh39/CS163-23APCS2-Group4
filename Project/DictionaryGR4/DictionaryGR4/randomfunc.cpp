// rng.cpp
#include "randomfunc.h"


// Initialize the RNG
std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

int RandInt(int l, int r)
{
	return (l + rng() % (r - l + 1));
}
