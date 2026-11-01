//
// Created by Dmitri on 2026-10-30.
//

#ifndef CONFLICT_MAP_ACLED_H
#define CONFLICT_MAP_ACLED_H
#include <vector>
#include <glm/vec3.hpp>

#include "../core/objects/Marker.h"


class ACLED {
public:
    static std::vector<MarkerInstance> GetConflictPoints(const char* jsonPath);
};


#endif //CONFLICT_MAP_ACLED_H
