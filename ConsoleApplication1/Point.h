#pragma once
#include "Random.h"


class Point
{
protected:
	double x_, y_;

public:
	Point ();
	Point (double x, double y);

	double getX () const;
	double getY () const;

	/*double& operator[] (int i);
	double operator[] (int i) const;*/

	static Point genPoint (Random& rand);

	friend double distance (const Point& p1, const Point& p2);
};

