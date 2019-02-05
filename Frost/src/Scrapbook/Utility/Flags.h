#pragma once

namespace sb
{
	class Flags
	{
	private:
		unsigned int m_flags;

	public:
		inline void addFlag(unsigned int flag)
		{
			m_flags |= flag;
		}

		inline void removeFlag(unsigned int flag)
		{
			m_flags &= ~flag;
		}

		inline bool hasFlag(unsigned int flag)
		{
			return ((m_flags & flag) == flag);
		}
	};
}