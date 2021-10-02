#include "Random.h"

Random::Random(int min, int max)
	: min_(min)
	, max_(max)
	, rng_(dev_())
	, dist_(min_, max_)
{}

int Random::Min() const {
	return min_;
}

int Random::Max() const {
	return max_;
}

int Random::Next() {
	return dist_(rng_);
}