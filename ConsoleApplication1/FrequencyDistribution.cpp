#include "FrequencyDistribution.h"


std::tuple<int, int, double> FrequencyDistribution::closestPoints (const std::vector<BM>& bms)
{
	auto res = twoClosestPoints (bms);

	return std::make_tuple (
		std::distance (bms.begin (), std::find (bms.begin (), bms.end (), std::get<0> (res))),
		std::distance (bms.begin (), std::find (bms.begin (), bms.end (), std::get<1> (res))),
		std::get<2> (res)
	);
}


std::tuple<BM, BM, double> FrequencyDistribution::bruteForce (const std::vector<BM>& bms)
{
	std::tuple<BM, BM, double> res;
	double min = std::numeric_limits<double>::max ();
	for (int i = 0; i < bms.size () - 1; ++i)
		for (int j = i + 1; j < bms.size (); ++j) {
			auto temp = dist (bms[i], bms[j]);
			if (temp < min) {
				res = std::make_tuple (bms[i], bms[j], temp);
				min = temp;
			}
		}

	return res;
}


std::tuple<BM, BM, double> FrequencyDistribution::stripClosest (std::vector<BM>& strip, double d)
{
	std::tuple<BM, BM, double> res;
	std::get<2> (res) = std::numeric_limits<double>::max ();
	double min = d;
	std::sort (strip.begin (), strip.end (), cmp_y);

	for (int i = 0; i < strip.size (); ++i)
		for (int j = i + 1; j < strip.size () && (strip[j].getY () - strip[i].getY ()) < min; ++j) {
			auto temp = dist (strip[i], strip[j]);
			if (temp < min) {
				res = std::make_tuple (strip[i], strip[j], temp);
				min = temp;
			}
		}

	return res;
}


std::tuple<BM, BM, double> FrequencyDistribution::twoClosestPoints (const std::vector<BM>& bms)
{
	int n = bms.size ();
	if (n <= 3)
		return bruteForce (bms);

	int mid = n >> 1;
	BM midPoint = bms[mid];

	std::vector<BM> x1 (bms.begin (), bms.begin () + mid);
	std::vector<BM> x2 (bms.begin () + mid, bms.end ());
	auto dl = twoClosestPoints (x1);
	auto dr = twoClosestPoints (x2);

	auto min = std::get<2> (dl) < std::get<2> (dr) ? dl : dr;
	double d = std::get<2> (min);

	std::vector<BM> strip;
	for (int i = 0; i < n; ++i)
		if (fabs (bms[i].getX () - midPoint.getX ()) < d)
			strip.push_back (bms[i]);

	auto tempRes = stripClosest (strip, d);

	if (d < std::get<2> (tempRes)) {
		return min;
	} else {
		return tempRes;
	}
}


std::pair<int, double> FrequencyDistribution::findClosest (const std::vector<BM>& bms, int chosen, const std::vector<int>& indexes)
{
	double minD = std::numeric_limits<double>::max ();
	int minIndex = 0;
	for (int i = chosen + 1; i < bms.size (); ++i) {
		auto it = std::find (indexes.begin (), indexes.end (), i);
		if (it != indexes.end ())
			continue;

		auto dx = bms[i].getX () - bms[chosen].getX ();
		if (dx * dx > minD)
			break;

		auto curr = dist (bms[i], bms[chosen]);
		if (curr < minD) {
			minD = curr;
			minIndex = i;
		}
	}

	for (int i = chosen - 1; i >= 0; --i) {
		auto it = std::find (indexes.begin (), indexes.end (), i);
		if (it != indexes.end ())
			continue;

		auto dx = bms[i].getX () - bms[chosen].getX ();
		if (dx * dx > minD)
			break;

		auto curr = dist (bms[i], bms[chosen]);
		if (curr < minD) {
			minD = curr;
			minIndex = i;
		}
	}

	return std::make_pair (minIndex, minD);
}


std::pair<int, double> FrequencyDistribution::findFarest (const std::vector<BM>& bms, int chosen, const std::vector<int>& indexes)
{
	double maxD = std::numeric_limits<double>::min ();

	int maxIndex = 0;
	for (int i = bms.size () - 1; i >= chosen + 1; --i) {
		auto it = std::find (indexes.begin (), indexes.end (), i);
		if (it != indexes.end ())
			continue;

		auto dx = bms[i].getX () - bms[chosen].getX ();
		if (dx * dx < maxD)
			break;

		auto curr = dist (bms[i], bms[chosen]);
		if (curr > maxD) {
			maxD = curr;
			maxIndex = i;
		}
	}

	for (int i = 0; i < chosen; ++i) {
		auto it = std::find (indexes.begin (), indexes.end (), i);
		if (it != indexes.end ())
			continue;

		auto dx = bms[i].getX () - bms[chosen].getX ();
		if (dx * dx < maxD)
			break;

		auto curr = dist (bms[i], bms[chosen]);
		if (curr > maxD) {
			maxD = curr;
			maxIndex = i;
		}
	}

	return std::make_pair (maxIndex, maxD);
}
