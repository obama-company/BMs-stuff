#include <iostream>
#include <iomanip>

#include "Battery.h"


int main ()
{
	size_t typeFirstCount = 0, typeSecondCount = 0;

	std::vector<Battery> bms;
	for (auto i = 0; i < 2; ++i) {
		if (i == 0) {
			std::cout << "Enter BM type 1 count: ";
			std::cin >> typeFirstCount;
			bms.push_back (Battery (typeFirstCount, 0, 1));		//BMs with random coordinates (add false as the fourth argument to use manual input)
		} else if (typeFirstCount != 6) {
			std::cout << "Enter BM type 2 count (have to be less or equal than " << 6 - typeFirstCount << "): ";
			std::cin >> typeSecondCount;
			if (typeSecondCount > 6 - typeFirstCount)
				typeSecondCount = 6 - typeFirstCount;

			bms.push_back (Battery (typeSecondCount, 1, 4));	//BMs with random coordinates (add false as the fourth argument to use manual input)
		}
	}
		
	for (auto i = 0; i < bms.size(); ++i)
		bms[i].makeDistribution();

	
	for (const auto& i : bms) {
		std::cout << i;
	}
}











/*int findClosestOrFarest (BM* bms, int chosen, size_t n, bool isClosest)
{
	if (isClosest) {
		double minD = std::numeric_limits<double>::max();  // квадрат расстояния
		int minIndex = 0;
		for (int i = chosen + 1; i < n; i++) {
			auto dx = bms[i].getX () - bms[chosen].getX ();
			if (dx * dx > minD)
				break;

			auto curr = dist (bms[i], bms[chosen]);
			if (curr < minD) {
				minD = curr;
				minIndex = i;
			}
		}

		for (int i = chosen - 1; i >= 0; i++) {
			auto dx = bms[i].getX () - bms[chosen].getX ();
			if (dx * dx > minD)
				break;

			auto curr = dist (bms[i], bms[chosen]);
			if (curr < minD) {
				minD = curr;
				minIndex = i;
			}
		}

		return minIndex;
	}
	else {
		double maxD = std::numeric_limits<double>::min ();

		int maxIndex = 0;
		for (int i = n - 1; i >= chosen + 1; i++) {
			auto dx = bms[i].getX () - bms[chosen].getX ();
			if (dx * dx < maxD)
				break;

			auto curr = dist (bms[i], bms[chosen]);
			if (curr > maxD) {
				maxD = curr;
				maxIndex = i;
			}
		}

		for (int i = 0; i < chosen; i++) {
			auto dx = bms[i].getX () - bms[chosen].getX ();
			if (dx * dx < maxD)
				break;

			auto curr = dist (bms[i], bms[chosen]);
			if (curr > maxD) {
				maxD = curr;
				maxIndex = i;
			}
		}

		return maxIndex;
	}
}
*/