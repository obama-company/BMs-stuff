#include <iomanip>

#include "BM.h"


BM::BM () : Point (), Wave () {}
BM::BM ( const Point& p, Wave wave, size_t id) : Point (p), Wave (wave), id_ (id) {}
BM::BM ( const BM& bm ) : Point ( bm ), Wave ( bm ), id_ (bm.id_) {}


size_t BM::getId () const
{
	return id_;
}


bool BM::operator==(const BM& op) const
{
	return x_ == op.x_ && y_ == op.y_ && value_ == op.value_ && type_ == op.type_;
}


std::ostream& operator<< ( std::ostream& out, const BM& bm )
{
	out << std::left << std::setw (15) << bm.x_ << std::setw (15) << bm.y_
		<< std::setw (6) << (int)bm.type_
		<< std::setw (8) << bm.value_
		<< std::endl;
	
	return out;
}
