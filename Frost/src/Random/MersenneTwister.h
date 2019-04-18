#pragma once

#include <stdint.h>

// Based on the Mersenne Twister algorithm
// This code is based on implenentation by Rez Graham
class MersenneTwister
{
private:
	static constexpr int RAND_N = 624;

	unsigned int m_seed;
	unsigned long m_mt[RAND_N];
	int m_mti;

public:
	MersenneTwister();

	void seed();
	void seed(unsigned int seed);
	unsigned int getSeed() const { return m_seed; }

	unsigned int rand();
	uint64_t rand64();
};