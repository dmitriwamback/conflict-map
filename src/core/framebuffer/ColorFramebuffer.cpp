//
// Created by Dmitri on 2026-10-30.
//

#include "ColorFramebuffer.h"

#include <iostream>
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>

void ColorFramebuffer::Initialize() {

    auto* gl = QOpenGLContext::currentContext()->extraFunctions();

    gl->glGenFramebuffers(1, &framebufferID);
    gl->glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    gl->glGenTextures(1, &colorTextureID);
    gl->glBindTexture(GL_TEXTURE_2D, colorTextureID);

    gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    gl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTextureID, 0);

    gl->glGenRenderbuffers(1, &depthbufferID);
    gl->glBindRenderbuffer(GL_RENDERBUFFER, depthbufferID);
    gl->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _width, _height);

    gl->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbufferID);

    if (gl->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is not complete!" << std::endl;
    }



    gl->glGenFramebuffers(1, &msaaFramebufferID);
    gl->glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebufferID);

    gl->glGenRenderbuffers(1, &msaaColorBufferID);
    gl->glBindRenderbuffer(GL_RENDERBUFFER, msaaColorBufferID);

    gl->glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA8, _width, _height);
    gl->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, msaaColorBufferID);

    gl->glGenRenderbuffers(1, &msaaDepthbufferID);
    gl->glBindRenderbuffer(GL_RENDERBUFFER, msaaDepthbufferID);

    gl->glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT24, _width, _height);
    gl->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, msaaDepthbufferID);

    if (gl->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "MSAA Framebuffer is not complete!" << std::endl;
    }

    gl->glBindTexture(GL_TEXTURE_2D, 0);
    gl->glBindRenderbuffer(GL_RENDERBUFFER, 0);
    gl->glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ColorFramebuffer::Bind() {
    auto* gl = QOpenGLContext::currentContext()->extraFunctions();
    gl->glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebufferID);
}

void ColorFramebuffer::Unbind() {
    auto* gl = QOpenGLContext::currentContext()->extraFunctions();

    gl->glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFramebufferID);
    gl->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebufferID);

    gl->glBlitFramebuffer(0, 0, _width, _height, 0, 0, _width, _height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    gl->glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

uint32_t ColorFramebuffer::GetTextureID() {
    return colorTextureID;
}

void ColorFramebuffer::Update(int width, int height) {

    _width = width;
    _height = height;

    auto* gl = QOpenGLContext::currentContext()->extraFunctions();

    gl->glBindTexture(GL_TEXTURE_2D, colorTextureID);
    gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    gl->glBindRenderbuffer(GL_RENDERBUFFER, depthbufferID);
    gl->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

    gl->glBindTexture(GL_TEXTURE_2D, 0);
    gl->glBindRenderbuffer(GL_RENDERBUFFER, 0);

    gl->glBindRenderbuffer(GL_RENDERBUFFER, msaaColorBufferID);

    gl->glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA8, width, height);

    gl->glBindRenderbuffer(GL_RENDERBUFFER, msaaDepthbufferID);
    gl->glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT24, width, height);
    gl->glBindTexture(GL_TEXTURE_2D, 0);
    gl->glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
