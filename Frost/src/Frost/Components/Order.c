#include "Order.h"

int ZIndexCmp(const ZIndex* left, const ZIndex* right)
{
	return left->z_index - right->z_index;
}
