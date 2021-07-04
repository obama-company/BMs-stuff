#include <iostream>
#include <iomanip>

#include "Battery.h"

 
int main ()
{
	bool isRandom;
	int count;

	Battery bms(1, 4);
	
	std::cout << "Generate coodinates randomly? [1/0]\n"; std::cin >> isRandom;
	
	std::cout << "Enter amount of BMs (have to be less or equal than 6): "; std::cin >> count;
	count = (count > 6) ? 6 : count;

	for (int i = 0; i < count; ++i) {
		Random rand;
		int t;
		std::cout << "Enter wave type (0/1): "; std::cin >> t;
		if (isRandom)
			bms.add (BM (Point::genPoint (rand), Wave (t), i));
		else {
			double x, y;
			std::cout << "enter x = "; std::cin >> x;
			std::cout << "enter y = "; std::cin >> y;
			bms.add (BM (Point (x, y), Wave (t), i));
		}
	}
		
	bms.makeDistribution ();

	std::cout << bms;

	return 0;
}