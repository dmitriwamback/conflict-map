//
// Created by Dmitri on 2026-10-30.
//

#ifndef CONFLICT_MAP_SHADER_H
#define CONFLICT_MAP_SHADER_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Shader {
public:
    void Create(const char *vertexShaderSource, const char *fragmentShaderSource);
    void Bind();
    void SetVec3(const char* name, const glm::vec3& value);
    void SetMat4(const char* name, const glm::mat4& value);
    void SetInt(const char* name, int value);

private:
    int program;
    unsigned int CompileShader(unsigned int type, const char *source);
};


#endif //CONFLICT_MAP_SHADER_H
