#pragma once
#include "BM.h"

namespace FrequencyDistribution
{
	inline bool cmp_x (const BM& p1, const BM& p2)
	{
		return p1.getX () < p2.getX () || p1.getX () == p2.getX () && p1.getY () < p2.getY ();
	}


	inline bool cmp_y (const BM& p1, const BM& p2)
	{
		return p1.getY () < p2.getY ();
	}


	inline double dist (const BM& p1, const BM& p2)
	{
		return (p1.getX () - p2.getX ()) * (p1.getX () - p2.getX ())
			+ (p1.getY () - p2.getY ()) * (p1.getY () - p2.getY ());
	}


	std::tuple<BM, BM, double> bruteForce (const std::vector<BM>& bms);
	std::tuple<BM, BM, double> stripClosest (std::vector<BM>& strip, double d);
	std::tuple<BM, BM, double> twoClosestPoints (const std::vector<BM>& bms);
	std::tuple<int, int, double> closestPoints (const std::vector<BM>& bms);
	std::pair<int, double> findClosest (const std::vector<BM>& bms, int chosen, const std::vector<int>& indexes);
	std::pair<int, double> findFarest (const std::vector<BM>& bms, int chosen, const std::vector<int>& indexes);
};

