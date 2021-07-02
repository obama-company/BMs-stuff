#include "Point.h"


Point::Point () : x_ ( 0 ), y_ ( 0 ) {}


Point::Point ( double x, double y ) : x_ (x), y_ (y) {}


double Point::getX () const { return x_; }


double Point::getY () const { return y_; }


Point Point::genPoint (Random& rand )
{
	return {rand.getDouble (), rand.getDouble ()};
}


std::ostream& print ( std::ostream& out, const Point& p )
{
	out << "(" << p.x_ << ", " << p.y_ << ")";
	return out;
}
