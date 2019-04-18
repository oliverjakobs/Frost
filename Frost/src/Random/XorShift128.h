#pragma once

#include <stdint.h>

// xorshift128+
// https://en.wikipedia.org/wiki/Xorshift
class XorShift128
{
public:
	struct Seed128
	{
		uint64_t state[2];

		Seed128() = default;
		constexpr Seed128(uint64_t upper, uint64_t lower) : state{ upper, lower } {}
		Seed128(const char* seedStr);
		constexpr Seed128(const Seed128& copy) : state{ copy.state[0], copy.state[1] } {}
		constexpr Seed128& operator=(const Seed128& copy) { state[0] = copy.state[0]; state[1] = copy.state[1]; return *this; }

		uint64_t upper() const { return state[0]; }
		uint64_t lower() const { return state[1]; }
	};
	
private:
	Seed128 m_seed;

public:
	XorShift128() = default;

	void seed();
	void seed(Seed128 seed) { m_seed = seed; }
	void seed(unsigned int seed);
	Seed128 getSeed() const { return m_seed; }

	unsigned int rand() { return static_cast<unsigned int>(rand64()); }
	uint64_t rand64();
};