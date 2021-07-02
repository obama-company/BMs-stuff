#include "Random.h"


Random::Random ()
{
	std::random_device device;
	random_generator_.seed (device ());
}


double Random::getDouble (double min, double max)
{
	std::uniform_real_distribution<double> range (min, max);
	return range (random_generator_);
}