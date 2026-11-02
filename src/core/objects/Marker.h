//
// Created by Dmitri on 2026-10-31.
//

#ifndef CONFLICT_MAP_MARKER_H
#define CONFLICT_MAP_MARKER_H
#include <QString>
#include <vector>
#include <glm/vec3.hpp>

enum class MarkerSource {
    UCDP,
    ACLED
};

struct MarkerData {
    MarkerSource source;
    QString date;
    QString country;
    QString location;
    QString eventType;
    int fatalities;
    int eventCount;
};

struct HoveredMarker {
    QString source;
    size_t index;
    float distanceSquared;
    float yearNormalized;
};

struct MarkerInstance {
    glm::vec3 position;
    float yearNormalized;
    size_t dataIndex;
};

class Marker {
public:
    void Initialize();
    void Render();
    void SetInstances(const std::vector<MarkerInstance>& instances);
    std::vector<MarkerInstance> GetInstances();

private:
    std::vector<glm::vec3> vertices;
    uint32_t vertexArrayObject;
    uint32_t vertexBufferObject;
    uint32_t instanceBufferObject;

    std::vector<MarkerInstance> instances;
};


#endif //CONFLICT_MAP_MARKER_H
