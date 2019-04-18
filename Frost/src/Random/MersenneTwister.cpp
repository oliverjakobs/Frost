#include "MersenneTwister.h"

#include <ctime>

MersenneTwister::MersenneTwister()
	: m_seed(1), m_mti(RAND_N + 1)
{
}

void MersenneTwister::seed()
{
	seed(static_cast<unsigned int>(std::time(nullptr)));
}

void MersenneTwister::seed(unsigned int seed)
{
	// setting initial seed to mt[N] using the generator Line 25 of Table 1 in
	// [KNUTH 1981, The Art of Computer Programming Vol.2 (2nd Ed.), pp102]
	m_mt[0] = seed & 0xffffffff;
	for (m_mti = 1; m_mti < RAND_N; ++m_mti)
		m_mt[m_mti] = (69069 * m_mt[m_mti - 1]) & 0xffffffff;

	m_seed = seed;
}

unsigned int MersenneTwister::rand()
{
#define RAND_TEMPERING_SHIFT_U(y) ((y) >> 11)
#define RAND_TEMPERING_SHIFT_S(y) ((y) << 7)
#define RAND_TEMPERING_SHIFT_T(y) ((y) << 15)
#define RAND_TEMPERING_SHIFT_L(y) ((y) >> 18)

	// period parameters
	constexpr int RAND_M = 397;
	constexpr unsigned long RAND_MATRIX_A = 0x9908b0df; // constant vector a
	constexpr unsigned long RAND_UPPER_MASK = 0x80000000; // most significant w-r bits
	constexpr unsigned long RAND_LOWER_MASK = 0x7fffffff; // least significant r bits

	constexpr unsigned long RAND_TEMPERING_MASK_B = 0x9d2c5680;
	constexpr unsigned long RAND_TEMPERING_MASK_C = 0xefc60000;

	unsigned long y;
	static unsigned long mag01[2] = { 0x0, RAND_MATRIX_A };

	if (m_mti >= RAND_N)
	{
		// generate N words at one time
		int kk;

		// if the RNG has not been seeded, default initial seed is used
		if (m_mti == RAND_N + 1)
			seed(4357);

		for (kk = 0; kk < RAND_N - RAND_M; kk++)
		{
			y = (m_mt[kk] & RAND_UPPER_MASK) | (m_mt[kk + 1] & RAND_LOWER_MASK);
			m_mt[kk] = m_mt[kk + RAND_M] ^ (y >> 1) ^ mag01[y & 0x1];
		}

		for (; kk < RAND_N - 1; kk++)
		{
			y = (m_mt[kk] & RAND_UPPER_MASK) | (m_mt[kk + 1] & RAND_LOWER_MASK);
			m_mt[kk] = m_mt[kk + (RAND_M - RAND_N)] ^ (y >> 1) ^ mag01[y & 0x1];
		}

		y = (m_mt[RAND_N - 1] & RAND_UPPER_MASK) | (m_mt[0] & RAND_LOWER_MASK);
		m_mt[RAND_N - 1] = m_mt[RAND_M - 1] ^ (y >> 1) ^ mag01[y & 0x1];

		m_mti = 0;
	}

	y = m_mt[m_mti++];
	y ^= RAND_TEMPERING_SHIFT_U(y);
	y ^= RAND_TEMPERING_SHIFT_S(y) & RAND_TEMPERING_MASK_B;
	y ^= RAND_TEMPERING_SHIFT_T(y) & RAND_TEMPERING_MASK_C;
	y ^= RAND_TEMPERING_SHIFT_L(y);

	return y;

#undef RAND_TEMPERING_SHIFT_U
#undef RAND_TEMPERING_SHIFT_S
#undef RAND_TEMPERING_SHIFT_T
#undef RAND_TEMPERING_SHIFT_L
}

uint64_t MersenneTwister::rand64()
{
	return uint64_t();
}
