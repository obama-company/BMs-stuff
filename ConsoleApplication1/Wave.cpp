#include "Wave.h"


Wave::Wave () : type_ ( WaveType::FIRST ), value_ ( 0 ) {}


Wave::Wave ( WaveType type ) : type_ (type), value_ ( 0 ) {}
Wave::Wave ( WaveType type, int value ) : type_ (type), value_ (value) {}


void Wave::setWaveValue (int value)
{
	value_ = value;
}


Wave::WaveType Wave::getWaveType () const { return type_; }


int Wave::getWaveValue () const { return value_; }


std::ostream& print( std::ostream& out, const Wave& wave )
{
	out << "Type: " << (int)wave.type_;
	if (wave.value_ != 0)
		out << "\tValue = " << wave.value_;

	return out;
}
