#include <iostream>
#include <tuple>

#include "BM.h"
#include "Random.h"

Random g_rand;


inline bool cmp_x (const BM& p1, const BM& p2)
{
	return p1.getX () < p2.getX () || p1.getX () == p2.getX () && p1.getY () < p2.getY ();
}


inline bool cmp_y (const BM& p1, const BM& p2)
{
	return p1.getY () < p2.getY ();
}


double dist (BM p1, BM p2)
{
	return sqrt ((p1.getX() - p2.getX()) * (p1.getX() - p2.getX()) 
		+ (p1.getY() - p2.getY()) * (p1.getY() - p2.getY()));
}


std::tuple<int, int, double> stripClosest (BM* strip, size_t size, double d)
{
	std::tuple<int, int, double> res;
	double min = d;
	std::sort (strip, strip + size, cmp_y);

	for (int i = 0; i < size; ++i)
		for (int j = i + 1; j < size && (strip[j].getY() - strip[i].getY()) < min; ++j)
		{
			auto temp = dist (strip[i], strip[j]);
			if (temp < min) {
				res = std::make_tuple (i, j, temp);
				min = temp;
			}
		}

	return res;
}


std::tuple<int, int, double> bruteForce (BM* bms, size_t size)
{
	std::tuple<int, int, double> res;
	double min = std::numeric_limits<double>::max();
	for (int i = 0; i < size; ++i)
		for (int j = i + 1; j < size; ++j)
		{
			auto temp = dist (bms[i], bms[j]);
			if (temp < min)
			{
				res = std::make_tuple(i, j, temp);
				min = temp;
			}
		}
	
	return res;
}


std::tuple<int, int, double> closestUtil (BM* bms, size_t n)
{
    if (n <= 3)
        return bruteForce (bms, n);

    int mid = n >> 1;
    BM midPoint = bms[mid];

    auto dl = closestUtil (bms, mid);
    auto dr = closestUtil (bms + mid, n - mid);

	auto min = std::get<2> (dl) < std::get<2> (dr) ? dl : dr;
    double d = std::get<2> (min);

    BM* strip = new BM[n];

    int j = 0;
    for (int i = 0; i < n; i++)
        if (fabs (bms[i].getX() - midPoint.getX()) < d)
            strip[j] = bms[i], j++;

	auto tempRes = stripClosest (strip, n, d);
	delete [] strip;

	if (d < std::get<2> (tempRes)) {
		return min;
	}
	else {
		return tempRes;
	}
}


std::pair<int, double> findClosest (BM* bms, int chosen, size_t n)
{
	double minD = std::numeric_limits<double>::max ();  // квадрат расстояния
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

	for (int i = chosen - 1; i >= 0; i--) {
		auto dx = bms[i].getX () - bms[chosen].getX ();
		if (dx * dx > minD)
			break;

		auto curr = dist (bms[i], bms[chosen]);
		if (curr < minD) {
			minD = curr;
			minIndex = i;
		}
	}

	return std::make_pair(minIndex, minD);
}


std::pair<int, double> findFarest (BM* bms, int chosen, size_t n)
{
	double maxD = std::numeric_limits<double>::min ();

	int maxIndex = 0;
	for (int i = n - 1; i >= chosen + 1; i--) {
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

	return std::make_pair(maxIndex, maxD);
}





void makeDistribution (BM* bms, size_t n, int type)
{
	if (n > 3) {
		std::sort (bms, bms + n, cmp_x);
		std::vector<int> picked (n);

		auto res = closestUtil (bms, n);
		picked[0] = std::get<0> (res);
		picked[1] = std::get<1> (res);

		auto firstVar = findClosest (bms, picked[0], n);
		auto secondVar = findClosest (bms, picked[1], n);
		if (firstVar.second > secondVar.second)
			picked[2] = secondVar.first;
		else
			picked[2] = firstVar.first;

		for (int i = 0; i < 3; ++i)
			bms[picked[i]].setWaveValue (type + i);

		double maxD = std::numeric_limits<double>::min ();
		for (int i = 0; i < 3; ++i) {
			auto temp = findFarest (bms, picked[i], n);
			if (temp.second > maxD) {
				picked[3] = temp.first;
				bms[picked[3]].setWaveValue (bms[picked[i]].getWaveValue());
				maxD = temp.second;
			}
		}

		if (n > 4) {
			firstVar = findFarest (bms, picked[0], n);
			secondVar = findFarest (bms, picked[1], n);
			if (firstVar.second > secondVar.second) {
				picked[4] = firstVar.first;
				bms[picked[4]].setWaveValue (bms[picked[0]].getWaveValue ());
			} else {
				picked[4] = secondVar.first;
				bms[picked[4]].setWaveValue (bms[picked[1]].getWaveValue ());
			}
		}

	} else {
		for (auto i = 0; i < 3; ++i)
			bms[i].setWaveValue (i + type);
	}
}


int main ()
{
	size_t typeFirstCount = 0, typeSecondCount = 0;

	BM** bms = new BM* [2];
	for (auto i = 0; i < 2; i++) {
		if (i == 0) {
			std::cout << "Enter BM type 1 count: ";
			std::cin >> typeFirstCount;
			bms[i] = new BM[typeFirstCount];
			for (auto j = 0; j < typeFirstCount; j++)
				bms[i][j] = BM (Point::genPoint (g_rand), Wave (Wave::WaveType::FIRST), j);
		} else {
			std::cout << "Enter BM type 2 count: ";
			std::cin >> typeSecondCount;
			bms[i] = new BM[typeSecondCount];
			for (auto j = 0; j < typeSecondCount; j++)
				bms[i][j] = BM (Point::genPoint (g_rand), Wave (Wave::WaveType::SECOND), j);
		}
	}
		
	makeDistribution (bms[0], typeFirstCount, 1);
	makeDistribution (bms[1], typeSecondCount, 4);

	for (int i = 0; i < typeFirstCount; i++)
		std::cout << bms[0][i] << std::endl;

	for (int i = 0; i < typeSecondCount; i++)
		std::cout << bms[1][i] << std::endl;
	
	//std::cout << res << std::endl;
	//std::cout << y << std::endl;
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