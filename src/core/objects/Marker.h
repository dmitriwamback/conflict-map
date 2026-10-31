//
// Created by Dmitri on 2026-10-31.
//

#ifndef CONFLICT_MAP_MARKER_H
#define CONFLICT_MAP_MARKER_H
#include <vector>
#include <glm/vec3.hpp>

struct MarkerInstance {
    glm::vec3 position;
};

class Marker {
public:
    void Initialize();
    void Render();
    void SetInstances(const std::vector<MarkerInstance>& instances);

private:
    std::vector<glm::vec3> vertices;
    uint32_t vertexArrayObject;
    uint32_t vertexBufferObject;
    uint32_t instanceBufferObject;

    std::vector<MarkerInstance> instances;
};


#endif //CONFLICT_MAP_MARKER_H
