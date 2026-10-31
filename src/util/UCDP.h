//
// Created by Dmitri on 2026-10-31.
//

#ifndef CONFLICT_MAP_UCDP_H
#define CONFLICT_MAP_UCDP_H
#include <vector>
#include <glm/vec3.hpp>


class UCDP {
public:
    static std::vector<glm::vec3> GetConflictPoints(const char* jsonPath);
};


#endif //CONFLICT_MAP_UCDP_H
