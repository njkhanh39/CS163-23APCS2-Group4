// my_functions.h
#ifndef GENERALFUNC
#define GENERALFUNC

#include <time.h>
#include <random>
#include <chrono>
#include <ctime>

// Function declaration
std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

int RandInt(int l, int r)
{
	return l + rng() % (r - l + 1);
}

#endif // MY_FUNCTIONS_H