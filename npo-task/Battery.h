#pragma once
#include "BM.h"

class Battery		//represents bms with the exact same wave type
{
public:
	Battery ();
	Battery (int freq, int freq2);
	Battery (const Battery& bat);

	void add (const BM& bm);
	void makeDistribution ();

	friend std::ostream& operator<< (std::ostream& out, const Battery& bat);

private:
	std::vector<std::vector<BM>> battery_;
	std::vector<int> freqs;		//starting freq (the other two are follow ups)

	static void makeDistribution (std::vector<BM>& vec, int freq);
};

