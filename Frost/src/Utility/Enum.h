#pragma once

#include <vector>
#include <string>

namespace detail
{
	struct ignore_assign
	{
		int value;

		ignore_assign(int v) : value(v) { }
		operator int() const { return value; }

		const ignore_assign& operator =(int dummy) { return *this; }
	};

	inline std::vector<std::string> process(std::vector<std::string> raw)
	{
		std::vector<std::string> processed;

		for (auto& name : raw) 
		{
			size_t length = std::strcspn(name.c_str(), " =\t\n\r");

			processed.push_back(name.substr(0, length));
		}

		return processed;
	}
}

#define EXPAND(expression) expression		// needed for the MSVC preprocessor 
#define APPLY(macro, ...) EXPAND(macro(__VA_ARGS__))

#define EVALUATE_COUNT(_1, _2, _3, _4, _5, _6, _7, _8, count, ...) count

#define COUNT(...) EXPAND(EVALUATE_COUNT(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1))

#define MAP(macro, ...) EXPAND(APPLY(MAP_START, COUNT(__VA_ARGS__))(macro, __VA_ARGS__))

#define MAP_START(count) MAP ## count

#define MAP1(m, x)      m(x)
#define MAP2(m, x, ...) m(x) EXPAND(MAP1(m, __VA_ARGS__))
#define MAP3(m, x, ...) m(x) EXPAND(MAP2(m, __VA_ARGS__))
#define MAP4(m, x, ...) m(x) EXPAND(MAP3(m, __VA_ARGS__))
#define MAP5(m, x, ...) m(x) EXPAND(MAP4(m, __VA_ARGS__))
#define MAP6(m, x, ...) m(x) EXPAND(MAP5(m, __VA_ARGS__))
#define MAP7(m, x, ...) m(x) EXPAND(MAP6(m, __VA_ARGS__))
#define MAP8(m, x, ...) m(x) EXPAND(MAP7(m, __VA_ARGS__))

#define IGNORE_ASSIGN_SINGLE(expression) (detail::ignore_assign)expression,
#define IGNORE_ASSIGN(...) EXPAND(MAP(IGNORE_ASSIGN_SINGLE, __VA_ARGS__))

#define STRINGIZE_SINGLE(expression) #expression,
#define STRINGIZE(...) EXPAND(MAP(STRINGIZE_SINGLE, __VA_ARGS__))

#define ENUM(EnumName, ...)												\
struct EnumName 														\
{																		\
    enum Enumerated { __VA_ARGS__ };									\
																		\
	Enumerated value;													\
																		\
	EnumName(Enumerated v) : value(v) { }								\
	EnumName(const std::string& str) : value(FromString(str)) { }		\
																		\
	operator Enumerated() const { return value; }						\
																		\
	static const size_t count = COUNT(__VA_ARGS__);						\
																		\
	static Enumerated FromString(const std::string& str)				\
	{																	\
		for (size_t index = 0; index < count; ++index)					\
		{																\
			if (Names()[index].compare(str) == 0)						\
				return (Enumerated)Values()[index];						\
		}																\
		return (Enumerated)0;											\
	}																	\
																		\
	std::string ToString() const										\
	{																	\
		for (size_t index = 0; index < count; ++index)					\
		{																\
			if (Values()[index] == value)								\
				return Names()[index];									\
		}																\
																		\
		return "";														\
	}																	\
																		\
	static std::vector<int> Values()									\
	{																	\
		return { EXPAND(IGNORE_ASSIGN(__VA_ARGS__)) };					\
	}																	\
																		\
	static std::vector<std::string> Names()								\
	{																	\
		return detail::process({ EXPAND(STRINGIZE(__VA_ARGS__)) });		\
	}																	\
};