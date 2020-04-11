#ifndef VISIBILITY_H
#define VISIBILITY_H

#include "math/math.h"

vec3* Visibility(vec2 pos, line* edges, size_t edge_count, size_t* intersection_count);

#endif /* !VISIBILITY_H */
