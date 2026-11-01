//
// Created by Dmitri on 2026-10-31.
//

#ifndef CONFLICT_MAP_MATH_H
#define CONFLICT_MAP_MATH_H
#include <glm/vec2.hpp>

inline glm::vec2 ToAzimuthalEquidistant(float lat, float lon) {
   float PI = 3.14159265358979323846f;
   float lat1 = PI / 2.0f;
   float lon1 = 0;

   float latNormalized = (lat + PI / 2.0f) / 2.0f;

   float k = 2;

   float x = k * cos(latNormalized) * sin(lon - lon1);
   float y = k * (cos(lat1) * sin(latNormalized) - sin(lat1) * cos(latNormalized) * cos(lon - lon1));

   return glm::vec2(x, y);
}

inline glm::vec2 ToMercator(float lat, float lon) {
   float PI = 3.14159265358979323846f;
   return glm::vec2(lon, log(tan(PI/4.0f + lat/2.0f)));
}

inline glm::vec2 ToWinkelTripel(double lat, double lon) {
   constexpr double PI = 3.14159265358979323846;
   constexpr double PHI1 = 0.594367;

   double deltaLon = lon;
   double alpha = acos(cos(lat) * cos(deltaLon / 2.0));

   double sinc = (alpha == 0.0) ? 1.0 : sin(alpha) / alpha;

   double xA = (2.0 * cos(lat) * sin(deltaLon / 2.0)) / sinc;

   double yA = sin(lat) / sinc;

   double xE = deltaLon * cos(PHI1);
   double yE = lat;

   return glm::vec2((xA + xE) / 2.0, (yA + yE) / 2.0);
}

inline glm::vec2 Project(float lat, float lon) {
   return ToWinkelTripel(lat, lon);
}

inline float ToRadians(float degrees) {
   float PI = 3.14159265358979323846f;
   return degrees * PI / 180.0f;
}

#endif //CONFLICT_MAP_MATH_H
