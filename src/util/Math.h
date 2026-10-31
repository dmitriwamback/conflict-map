//
// Created by Dmitri on 2026-10-31.
//

#ifndef CONFLICT_MAP_MATH_H
#define CONFLICT_MAP_MATH_H
#include <glm/vec2.hpp>

inline glm::vec2 ToMercator(float lat, float lon) {
   float PI = 3.14159265358979323846f;
   return glm::vec2(lon, log(tan(PI/4.0f + lat/2.0f)));
}

inline float ToRadians(float degrees) {
   float PI = 3.14159265358979323846f;
   return degrees * PI / 180.0f;
}

#endif //CONFLICT_MAP_MATH_H
