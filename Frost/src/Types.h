#pragma once

#include <glm.hpp>
#include <string>
#include <map>
#include <vector>

#include "Utility/Enum.h"


// ------------------| BASICS |---------------------------------------------------------
typedef unsigned char byte;
typedef unsigned int uint;

typedef std::vector<glm::vec2> Vertices;

// ------------------| COLOR |----------------------------------------------------------
typedef glm::vec4 Color;

const Color BLACK = Color(0.0f, 0.0f, 0.0f, 1.0f);
const Color WHITE = Color(1.0f, 1.0f, 1.0f, 1.0f);
const Color RED = Color(1.0f, 0.0f, 0.0f, 1.0f);
const Color GREEN = Color(0.0f, 1.0f, 0.0f, 1.0f);
const Color BLUE = Color(0.0f, 0.0f, 1.0f, 1.0f);
const Color YELLOW = Color(1.0f, 1.0f, 0.0f, 1.0f);
const Color MAGENTA = Color(1.0f, 0.0f, 1.0f, 1.0f);
const Color CYAN = Color(0.0f, 1.0f, 1.0f, 1.0f);

inline Color BlendColor(const Color& color, float alpha)
{
	return Color(color.r, color.g, color.b, alpha);
}

// ------------------| SMART-POINTER |--------------------------------------------------
#include <memory>

using std::unique_ptr;
using std::make_unique;

using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;

// ------------------| ENUMS |----------------------------------------------------------
ENUM(RenderFlip, NONE = 0, HORIZONTAL = 1, VERTICAL = 2);

ENUM(Direction, LEFT, RIGHT, UP,  DOWN);