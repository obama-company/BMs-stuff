#pragma once
#include <iostream>
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

	static Point genPoint (Random& rand);

	friend std::ostream& print(std::ostream& out, const Point& p);
};

