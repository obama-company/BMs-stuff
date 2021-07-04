#include "Wave.h"


Wave::Wave () : type_ ( WaveType::FIRST ), value_ ( 0 ) {}
Wave::Wave ( WaveType type ) : type_ (type), value_ ( 0 ) {}
Wave::Wave ( WaveType type, int value ) : type_ (type), value_ (value) {}


Wave::Wave (int type)
{
	if (type == 0)
		type_ = WaveType::FIRST;
	else
		type_ = WaveType::SECOND;

	value_ = 0;
}


void Wave::setWaveValue (int value)
{
	value_ = value;
}


Wave::WaveType Wave::getWaveType () const { return type_; }


int Wave::getWaveValue () const { return value_; }
