//
// Created by Dmitri on 2026-10-30.
//

#ifndef CONFLICT_MAP_COLORFRAMEBUFFER_H
#define CONFLICT_MAP_COLORFRAMEBUFFER_H
#include <cstdint>


class ColorFramebuffer {
public:
    ColorFramebuffer() = default;

    void Initialize();
    void Bind();
    void Unbind();
    void Update(int width, int height);

    uint32_t GetTextureID();

private:
    uint32_t framebufferID = 0;
    uint32_t depthbufferID = 0;
    uint32_t colorTextureID = 0;

    uint32_t msaaFramebufferID = 0;
    uint32_t msaaDepthbufferID = 0;
    uint32_t msaaColorBufferID = 0;

    int _width = 1200;
    int _height = 800;
};


#endif //CONFLICT_MAP_COLORFRAMEBUFFER_H
