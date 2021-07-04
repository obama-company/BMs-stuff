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

	friend std::ostream& print(std::ostream& out, const Wave& wave);

protected:
	WaveType type_;
	int value_;
};

