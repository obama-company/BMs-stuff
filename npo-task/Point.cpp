#include "Point.h"
#include <iostream>


Point::Point () : x_ ( 0 ), y_ ( 0 ) {}


Point::Point ( double x, double y ) : x_ (x), y_ (y) {}


double Point::getX () const { return x_; }


double Point::getY () const { return y_; }


//double& Point::operator[](int i)
//{
//	if (i == 0)
//		return x_;
//	else if (i == 1)
//		return y_;
//	else
//		throw std::out_of_range ("");
//}
//
//double Point::operator[](int i) const
//{
//	if (i == 0)
//		return x_;
//	else if (i == 1)
//		return y_;
//	else
//		throw std::out_of_range ("");
//}


Point Point::genPoint (Random& rand )
{
	return {rand.getDouble (), rand.getDouble ()};
}


double distance (const Point& p1, const Point& p2)
{
	return (p1.x_ - p2.x_) * (p1.x_ - p2.x_) + (p1.y_ - p2.y_) * (p1.y_ - p2.y_);
}
