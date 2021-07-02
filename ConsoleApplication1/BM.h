#pragma once
#include "Point.h"
#include "Wave.h"


class BM : public Point, public Wave
{
public:
	BM ();
	BM (const Point& p, Wave wave, size_t id);
	BM (const BM& bm);

	void setWaveValue (int value);
	size_t getId () const;

	friend std::ostream& operator<< (std::ostream& out, const BM& bm);

private:
	size_t id_;
};

