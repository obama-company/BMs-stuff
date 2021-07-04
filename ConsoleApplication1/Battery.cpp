#include "Battery.h"
#include "FrequencyDistribution.h"

#include <iomanip>

using namespace FrequencyDistribution;

Battery::Battery () : frequency_ (0) {}


Battery::Battery (size_t size, int type, int freq, bool isRandom) : frequency_ (freq)
{
	if (type > 1) {
		std::cerr << "wrong type specified\n";
		return;
	}

	battery_.reserve (size);
	Random rand;
	Wave::WaveType t;
	if (type == 0)
		t = Wave::WaveType::FIRST;
	else
		t = Wave::WaveType::SECOND;
	for (auto i = 0; i < size; ++i) {
		if (isRandom)
			battery_.push_back (BM (Point::genPoint (rand), Wave (t), i));
		else {
			double x, y;
			std::cout << "enter x = "; std::cin >> x;
			std::cout << "enter y = "; std::cin >> y;
			battery_.push_back (BM (Point(x, y), Wave (t), i));
		}
	}
		
}


Battery::Battery (const Battery& bat) : frequency_ (bat.frequency_)
{
	battery_ = bat.battery_;
}


void Battery::makeDistribution ()
{
	if (battery_.size() > 3) {
		std::sort (battery_.begin (), battery_.end (), cmp_x);
		std::vector<int> picked;
		picked.reserve (battery_.size ());

		auto res = closestPoints (battery_);
		picked.push_back (std::get<0> (res));
		picked.push_back (std::get<1> (res));

		auto firstVar = findClosest (battery_, picked[0], picked);
		auto secondVar = findClosest (battery_, picked[1], picked);
		if (firstVar.second > secondVar.second)
			picked.push_back (secondVar.first);
		else
			picked.push_back (firstVar.first);

		for (int i = 0; i < 3; ++i)
			battery_[picked[i]].setWaveValue (frequency_ + i);

		int doubleUsed = 0;
		std::pair<int, double> maxRes = std::make_pair (-1, std::numeric_limits<double>::min ());
		for (int i = 0; i < picked.size (); ++i) {
			auto temp = findFarest (battery_, picked[i], picked);
			if (temp.second > maxRes.second) {
				maxRes = temp;
				doubleUsed = picked[i];
			}
		}
		picked.push_back (maxRes.first);
		battery_[picked[3]].setWaveValue (battery_[doubleUsed].getWaveValue ());


		if (battery_.size () > 4) {
			std::vector<int> uses;
			for (int i = 0; i < 3; ++i) {
				if (picked[i] != doubleUsed)
					uses.push_back (picked[i]);
			}

			firstVar = findFarest (battery_, uses[0], picked);
			secondVar = findFarest (battery_, uses[1], picked);

			if (firstVar.second > secondVar.second) {
				picked.push_back (firstVar.first);
				battery_[picked[4]].setWaveValue (battery_[uses[0]].getWaveValue ());
				doubleUsed = uses[1];	//now its the only unused
			} else {
				picked.push_back (secondVar.first);
				battery_[picked[4]].setWaveValue (battery_[uses[1]].getWaveValue ());
				doubleUsed = uses[0];	//now its the only unused
			}
		}

		if (battery_.size () > 5) {
			int leftover = 0;
			for (int i = 0; i < battery_.size (); ++i) {
				auto it = std::find (picked.begin (), picked.end (), i);
				if (it == picked.end ()) {
					leftover = i;
					break;
				}
			}

			auto closest = findClosest (battery_, leftover, {});
			auto farest = findFarest (battery_, closest.first, { leftover, doubleUsed });

			auto dst = dist (battery_[leftover], battery_[doubleUsed]);
			if (dst > farest.second)
				battery_[leftover].setWaveValue (battery_[doubleUsed].getWaveValue ());
			else
				battery_[leftover].setWaveValue (battery_[farest.first].getWaveValue ());
		}
	} else {
		for (auto i = 0; i < battery_.size (); ++i)
			battery_[i].setWaveValue (i + frequency_);
	}
}

std::ostream& operator<<(std::ostream& out, const Battery& bat)
{
	std::cout << std::left << std::setw (15) << "X" << std::setw (15) << "Y"
		<< std::setw (6) << "TYPE"
		<< std::setw (8) << "VALUE"
		<< std::endl;
	for (const auto& j : bat.battery_)
		std::cout << j;

	return out;
}
