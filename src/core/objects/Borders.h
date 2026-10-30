//
// Created by Dmitri on 2026-10-30.
//

#ifndef CONFLICT_MAP_BORDERS_H
#define CONFLICT_MAP_BORDERS_H

#include <QJsonArray>
#include <QJsonObject>
#include <glm/glm.hpp>


struct BorderLine {
    std::vector<glm::vec3> vertices;

    uint32_t vertexArrayObject = 0;
    uint32_t vertexBufferObject = 0;
};

class Borders {
public:
    bool Load(const QString& path);
    void Initialize();
    void Render() const;

private:
    void ParseGeometry(const QJsonObject& geometry);
    void ParseLineString(const QJsonArray& coordinates);

    std::vector<BorderLine> lines;
};


#endif //CONFLICT_MAP_BORDERS_H
