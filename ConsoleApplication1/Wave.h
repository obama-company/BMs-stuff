#pragma once
#include <iostream>

class Wave
{
public:
	enum class WaveType
	{
		FIRST,
		SECOND
	};

	Wave ();
	Wave (WaveType type);
	Wave (WaveType type, int value);

	void setWaveValue (int value);

	WaveType getWaveType () const;
	int getWaveValue () const;

protected:
	WaveType type_;
	int value_;
};

