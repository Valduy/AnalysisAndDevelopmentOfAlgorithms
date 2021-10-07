#ifndef HAMMURABI_RANDOM_H_
#define HAMMURABI_RANDOM_H_

#include <random>

class Random {
public:
	Random(int min, int max);

	int Min() const;
	int Max() const;
	int Next();

private:
	int min_;
	int max_;

	std::random_device dev_;
	std::mt19937 rng_;
	std::uniform_int_distribution<std::mt19937::result_type> dist_;
};

#endif HAMMURABI_RANDOM_H_
