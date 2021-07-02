#include "BM.h"


BM::BM () : Point (), Wave (){
}


BM::BM ( const Point& p, Wave wave, size_t id) : Point (p), Wave (wave), id_ (id) {}


BM::BM ( const BM& bm ) : Point ( bm ), Wave ( bm ), id_ (bm.id_) {}


void BM::setWaveValue (int value)
{
	value_ = value;
}


size_t BM::getId () const
{
	return id_;
}


std::ostream& operator<< ( std::ostream& out, const BM& bm )
{
	out << "(" << bm.x_ << ", " << bm.y_ << ")";
	out << "Type: " << (int)bm.type_;
	if (bm.value_ != 0)
		out << "\tValue = " << bm.value_;
	
	return out;
}
