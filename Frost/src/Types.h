#pragma once

#include "Ignis/Core/Api.h"


// ------------------| BASICS |---------------------------------------------------------
typedef unsigned char byte;
typedef unsigned int uint;

typedef std::vector<glm::vec2> Vertices;

// ------------------| COLOR |----------------------------------------------------------
inline ignis::color BlendColor(const ignis::color& color, float alpha)
{
	return ignis::color(color.r, color.g, color.b, alpha);
}

// ------------------| SMART-POINTER |--------------------------------------------------
#include <memory>

using std::unique_ptr;
using std::make_unique;

using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;