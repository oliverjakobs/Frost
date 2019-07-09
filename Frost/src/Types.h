#pragma once

#include <glm.hpp>
#include <string>
#include <map>
#include <vector>

#include "Utility/Enum.h"

typedef glm::vec4 Color;

typedef unsigned char byte;
typedef unsigned int uint;


typedef std::vector<glm::vec2> Vertices;

#include <memory>

using std::unique_ptr;
using std::make_unique;

using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;

ENUM(RenderFlip, NONE = 0, HORIZONTAL = 1, VERTICAL = 2);

ENUM(Direction, LEFT, RIGHT, UP,  DOWN);