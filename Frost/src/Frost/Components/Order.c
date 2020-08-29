#include "Order.h"

int TemplateCmp(const Template* left, const Template* right)
{
	return left->entity - right->entity;
}

int ZIndexCmp(const ZIndex* left, const ZIndex* right)
{
	return left->z_index - right->z_index;
}
