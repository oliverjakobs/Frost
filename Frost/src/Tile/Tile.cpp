#include "Tile.h"

#include "String/StringUtils.h"

BodyType FromString(const std::string& str)
{
	if (stringCompare(str, "BodyTypeStatic"))
		return BODY_STATIC;

	if (stringCompare(str, "BodyTypeDynamic"))
		return BODY_DYNAMIC;

	return BODY_STATIC;
}