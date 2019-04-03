#pragma once

// The MIT License(MIT)
// 
// Copyright(c) 2015 Stefan Reinalter
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <cstdint>
#include <string>

namespace compileTime
{
	template <size_t I>
	struct Hash
	{
		template <size_t N>
		constexpr static inline uint32_t Generate(const char(&str)[N])
		{
			return static_cast<uint32_t>(static_cast<uint64_t>(Hash<I - 1u>::Generate(str) ^ uint32_t(str[I - 1u])) * 16777619ull);
		}
	};

	template <>
	struct Hash<0u>
	{
		template <size_t N>
		constexpr static inline uint32_t Generate(const char(&str)[N])
		{
			return 2166136261u;
		}
	};
}


namespace runTime
{
	inline uint32_t Fnv1aHash(uint32_t hash, const char* str, size_t length)
	{
		for (size_t i = 0; i < length; ++i)
		{
			const uint32_t value = static_cast<uint32_t>(*str++);
			hash ^= value;
			hash *= 16777619u;
		}

		return hash;
	}

	inline uint32_t Fnv1aHash(const char* str, size_t length)
	{
		return Fnv1aHash(2166136261u, str, length);
	}

	inline uint32_t Fnv1aHash(const char* str)
	{
		return Fnv1aHash(str, strlen(str));
	}
}


namespace detail
{
	template <typename T>
	struct HashHelper {};

	template <>
	struct HashHelper<const char*>
	{
		static inline uint32_t Generate(const char* str)
		{
			return runTime::Fnv1aHash(str);
		}
	};

	template <size_t N>
	struct HashHelper<char[N]>
	{
		constexpr static inline uint32_t Generate(const char(&str)[N])
		{
			return compileTime::Hash<N - 1u>::Generate(str);
		}
	};
}


template <typename T>
constexpr static inline uint32_t GenerateHash(const T& str)
{
	return detail::HashHelper<T>::Generate(str);
}


class StringHash
{
public:
	template <typename T>
	StringHash(const T& str)
		: m_hash(GenerateHash(str))
	{
	}

	inline uint32_t GetHash(void) const
	{
		return m_hash;
	}

private:
	const uint32_t m_hash;
};
