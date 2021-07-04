#pragma once
#include "BM.h"

class Battery
{
public:
	Battery ();
	Battery (size_t size, int type, int freq, bool isRandom = true);
	Battery (const Battery& bat);

	void makeDistribution ();

	friend std::ostream& operator<< (std::ostream& out, const Battery& bat);

private:
	std::vector<BM> battery_;
	int frequency_;
};

