//
// Created by Dmitri on 2026-10-31.
//

#ifndef CONFLICT_MAP_UCDP_H
#define CONFLICT_MAP_UCDP_H
#include <vector>
#include <glm/vec3.hpp>

#include "../core/objects/Marker.h"


class UCDP {
public:
    static std::vector<MarkerInstance> GetConflictPoints(const char* jsonPath);
};


#endif //CONFLICT_MAP_UCDP_H
