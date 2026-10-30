//
// Created by Dmitri on 2026-10-30.
//

#ifndef CONFLICT_MAP_QUAD_H
#define CONFLICT_MAP_QUAD_H
#include <cstdint>


class Quad {
public:
    Quad() = default;
    void Initialize();
    void Render() const;

private:
    uint32_t vertexArrayObject;
    uint32_t vertexBufferObject;
};



#endif //CONFLICT_MAP_QUAD_H
