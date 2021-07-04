#pragma once
#include <random>

class Random
{
public:
	Random ();
	double getDouble (double min = -500, double max = 500);

private:
	std::mt19937 random_generator_;
};

