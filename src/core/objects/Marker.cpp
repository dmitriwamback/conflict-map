//
// Created by Dmitri on 2026-10-31.
//

#include "Marker.h"

#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>

#include <iostream>

void Marker::Initialize() {

    // Octagon Shape
    float sideLength = 0.0015f;
    float offset = sideLength/2.0f;
    float perpTriangleSideLength = sideLength/sqrt(2.0f);

    vertices = {
        // center square
        glm::vec3(-offset, -offset, 0), glm::vec3(offset, offset, 0), glm::vec3(offset, -offset, 0),
        glm::vec3(-offset, -offset, 0), glm::vec3(-offset, offset, 0), glm::vec3(offset, offset, 0),

        // left extrusion
        glm::vec3(-offset - perpTriangleSideLength, -offset, 0), glm::vec3(-offset, offset, 0), glm::vec3(-offset - perpTriangleSideLength, offset, 0),
        glm::vec3(-offset - perpTriangleSideLength, -offset, 0), glm::vec3(-offset, -offset, 0), glm::vec3(-offset, offset, 0),

        // right extrusion
        glm::vec3(offset, -offset, 0), glm::vec3(offset + perpTriangleSideLength, offset, 0), glm::vec3(offset, offset, 0),
        glm::vec3(offset, -offset, 0), glm::vec3(offset + perpTriangleSideLength, -offset, 0), glm::vec3(offset + perpTriangleSideLength, offset, 0),

        // top extrusion
        glm::vec3(-offset, offset, 0), glm::vec3(offset, offset, 0), glm::vec3(offset, offset + perpTriangleSideLength, 0),
        glm::vec3(-offset, offset, 0), glm::vec3(offset, offset + perpTriangleSideLength, 0), glm::vec3(-offset, offset + perpTriangleSideLength, 0),

        // bottom extrusion
        glm::vec3(-offset, -offset, 0), glm::vec3(-offset, -offset - perpTriangleSideLength, 0), glm::vec3(offset, -offset, 0),
        glm::vec3(-offset, -offset - perpTriangleSideLength, 0), glm::vec3(offset, -offset - perpTriangleSideLength, 0), glm::vec3(offset, -offset, 0),

        // top left triangle
        glm::vec3(-offset, offset, 0), glm::vec3(-offset - perpTriangleSideLength, offset, 0), glm::vec3(-offset, offset + perpTriangleSideLength, 0),

        // top right triangle
        glm::vec3(offset, offset, 0), glm::vec3(offset, offset + perpTriangleSideLength, 0), glm::vec3(offset + perpTriangleSideLength, offset, 0),

        // bottom left triangle
        glm::vec3(-offset - perpTriangleSideLength, -offset, 0), glm::vec3(-offset, -offset - perpTriangleSideLength, 0), glm::vec3(-offset, -offset, 0),

        // bottom right triangle
        glm::vec3(offset, -offset, 0), glm::vec3(offset, -offset - perpTriangleSideLength, 0), glm::vec3(offset + perpTriangleSideLength, -offset, 0)
    };

    auto* gl = QOpenGLContext::currentContext()->extraFunctions();

    gl->glGenVertexArrays(1, &vertexArrayObject);
    gl->glGenBuffers(1, &vertexBufferObject);

    gl->glBindVertexArray(vertexArrayObject);

    gl->glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    gl->glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(glm::vec3)), vertices.data(), GL_STATIC_DRAW);

    gl->glEnableVertexAttribArray(0);
    gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

    gl->glGenBuffers(1, &instanceBufferObject);

    gl->glBindBuffer(GL_ARRAY_BUFFER, instanceBufferObject);

    gl->glEnableVertexAttribArray(1);
    gl->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MarkerInstance), (void*)offsetof(MarkerInstance, position));
    gl->glVertexAttribDivisor(1, 1);

    gl->glEnableVertexAttribArray(2);
    gl->glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(MarkerInstance), (void*)offsetof(MarkerInstance, yearNormalized));
    gl->glVertexAttribDivisor(2, 1);

    gl->glBindVertexArray(0);
}

void Marker::SetInstances(const std::vector<MarkerInstance>& instances) {
    this->instances = instances;

    auto* gl = QOpenGLContext::currentContext()->extraFunctions();

    gl->glBindBuffer(GL_ARRAY_BUFFER, instanceBufferObject);
    gl->glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(instances.size() * sizeof(MarkerInstance)), instances.data(), GL_DYNAMIC_DRAW);
    gl->glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Marker::Render() {
    auto* gl = QOpenGLContext::currentContext()->extraFunctions();

    gl->glBindVertexArray(vertexArrayObject);
    gl->glDrawArraysInstanced(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()), static_cast<GLsizei>(instances.size()));
    gl->glBindVertexArray(0);
}

std::vector<MarkerInstance> Marker::GetInstances() {
    return instances;
}