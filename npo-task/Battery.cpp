#include <iomanip>

#include "Battery.h"
#include "FrequencyDistribution.h"

using namespace FrequencyDistribution;


Battery::Battery ()
{
	battery_.push_back (std::vector<BM> ());
	battery_.push_back (std::vector<BM> ());
}


Battery::Battery (int freq, int freq2)
{
	battery_.push_back (std::vector<BM> ());
	battery_.push_back (std::vector<BM> ());
	freqs.push_back (freq);
	freqs.push_back (freq2);
}


Battery::Battery (const Battery& bat)
{
	battery_ = bat.battery_;
	freqs = bat.freqs;
}


void Battery::add (const BM& bm)
{
	if (bm.getWaveType () == Wave::WaveType::FIRST)
		battery_[0].push_back (bm);
	else
		battery_[1].push_back (bm);
}


void Battery::makeDistribution ()
{
	for (int i = 0; i < battery_.size (); i++)
		makeDistribution (battery_[i], freqs[i]);
}


void Battery::makeDistribution (std::vector<BM>& vec, int freq)
{
	if (vec.size() > 3) {
		std::sort (vec.begin (), vec.end (), cmp_x);
		std::vector<int> picked;	//vector of already distributed bms
		picked.reserve (vec.size ());

		auto res = closestPoints (vec);
		picked.push_back (std::get<0> (res));	//adding two closest to each other points (a)
		picked.push_back (std::get<1> (res));

		auto firstVar = findClosest (vec, picked[0], picked);
		auto secondVar = findClosest (vec, picked[1], picked);

		if (firstVar.second > secondVar.second)	//adding closest to one of the two points to picked (b)
			picked.push_back (secondVar.first);
		else
			picked.push_back (firstVar.first);

		for (int i = 0; i < 3; ++i)	//distributing freqs (c)
			vec[picked[i]].setWaveValue (freq + i);

		int doubleUsed = 0;
		std::pair<int, double> maxRes = std::make_pair (-1, std::numeric_limits<double>::min ());	//finding farthest point
		for (int i = 0; i < picked.size (); ++i) {
			auto temp = findFarest (vec, picked[i], picked);
			if (temp.second > maxRes.second) {
				maxRes = temp;
				doubleUsed = picked[i];	//pointing bms with double used freq
			}
		}
		picked.push_back (maxRes.first);	//adding to picked
		vec[picked[3]].setWaveValue (vec[doubleUsed].getWaveValue ());	//setting freq to fourth bm (d)


		if (vec.size () > 4) {
			std::vector<int> uses;
			for (int i = 0; i < 3; ++i) {	//making vector of bms with double usused freqs
				if (picked[i] != doubleUsed)
					uses.push_back (picked[i]);
			}

			firstVar = findFarest (vec, uses[0], picked);
			secondVar = findFarest (vec, uses[1], picked);

			if (firstVar.second > secondVar.second) {	//setting freq to 5 bm (e)
				picked.push_back (firstVar.first);
				vec[picked[4]].setWaveValue (vec[uses[0]].getWaveValue ());
				doubleUsed = uses[1];	//now its the only unused
			} else {
				picked.push_back (secondVar.first);
				vec[picked[4]].setWaveValue (vec[uses[1]].getWaveValue ());
				doubleUsed = uses[0];	//now its the only unused
			}
		}

		if (vec.size () > 5) {	//finding leftover bm
			int leftover = 0;
			for (int i = 0; i < vec.size (); ++i) {
				auto it = std::find (picked.begin (), picked.end (), i);
				if (it == picked.end ()) {
					leftover = i;
					break;
				}
			}

			auto closest = findClosest (vec, leftover, {});
			auto farest = findFarest (vec, closest.first, { leftover, doubleUsed });	//finding fathrest among bms with double used freqs

			auto dst = dist (vec[leftover], vec[doubleUsed]);	//dist between lefover bm and bm with double unused freq
			if (dst > farest.second)	//comparing and setting sixth (f)
				vec[leftover].setWaveValue (vec[doubleUsed].getWaveValue ());
			else
				vec[leftover].setWaveValue (vec[farest.first].getWaveValue ());
		}
	} else {
		for (auto i = 0; i < vec.size (); ++i)
			vec[i].setWaveValue (i + freq);
	}
}


std::ostream& operator<< (std::ostream& out, const Battery& bat)
{
	std::cout << std::left << std::setw (15) << "X" << std::setw (15) << "Y"
		<< std::setw (6) << "TYPE"
		<< std::setw (8) << "VALUE"
		<< std::endl;
	for (auto i = 0; i < bat.battery_.size(); ++i)
		for (auto j = 0; j < bat.battery_[i].size(); ++j)
			std::cout << bat.battery_[i][j];

	return out;
}
