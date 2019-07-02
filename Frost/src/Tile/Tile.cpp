#include "Tile.h"

BodyType FromString(const std::string& str)
{
	if (stringCompare(str, "BodyTypeStatic"))
		return BodyTypeStatic;

	if (stringCompare(str, "BodyTypeDynamic"))
		return BodyTypeDynamic;

	return BodyTypeStatic;
}