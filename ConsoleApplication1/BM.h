#pragma once
#include "Point.h"
#include "Wave.h"


class BM : public Point, public Wave
{
public:
	BM ();
	BM (const Point& p, Wave wave, size_t id);
	BM (const BM& bm);

	size_t getId () const;

	bool operator== (const BM& op) const;

	friend std::ostream& operator<< (std::ostream& out, const BM& bm);

private:
	size_t id_;
};

