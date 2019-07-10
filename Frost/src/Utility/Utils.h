#pragma once

#include <iterator>
#include <utility>
#include <functional>

#define SAFE_DELETE(ptr) if( (ptr) != nullptr ) delete (ptr); (ptr) = nullptr;

#define BIT(x) (1 << x)

#define BIND_FUNCTION(fn) std::bind(&fn, this, std::placeholders::_1)

template <typename Type>
struct range_t
{
	Type b, e;
	range_t(Type x, Type y) : b(x), e(y) {}

	Type begin() { return b; }
	Type end() { return e; }
};

template <typename Type>
range_t<Type> range(Type b, Type e)
{
	return range_t<Type>(b, e);
}
