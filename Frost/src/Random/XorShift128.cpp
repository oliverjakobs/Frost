#include "XorShift128.h"

#include <ctime>
#include "Utility/Debugger.h"
#include "Utility/String/StringUtils.h"

XorShift128::Seed128::Seed128(const char* seedStr)
{
	auto split = cutString(":", seedStr);

	if (split.size() != 2)
	{
		DEBUG_ERROR("Invalid seedStr ({0})", seedStr);
		return;
	}

	state[0] = std::strtoull(split[0].c_str(), nullptr, 10);
	state[1] = std::strtoull(split[1].c_str(), nullptr, 10);
}

void XorShift128::seed()
{
	m_seed.state[0] = std::time(nullptr);
	m_seed.state[1] = std::time(nullptr);
}

void XorShift128::seed(unsigned int seed)
{
	m_seed.state[0] = 0;
	m_seed.state[1] = seed;
}

uint64_t XorShift128::rand64()
{
	uint64_t t = m_seed.state[0];
	const uint64_t s = m_seed.state[1];
	m_seed.state[0] = s;
	t ^= t << 23;		// a
	t ^= t >> 17;		// b
	t ^= s ^ (s >> 26);	// c
	m_seed.state[1] = t;
	return t + s;
}
