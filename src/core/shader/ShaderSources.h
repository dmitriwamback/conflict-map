//
// Created by Dmitri on 2026-10-30.
//

#ifndef CONFLICT_MAP_SHADERSOURCES_H
#define CONFLICT_MAP_SHADERSOURCES_H

static constexpr const char* BORDER_VERTEX_SHADER_SOURCE = R"(
    #version 330 core

    layout(location = 0) in vec3 position;

    uniform mat4 lookAt;
    uniform mat4 projection;
    uniform mat4 model;

    out vec3 fragp;

    void main() {
        fragp = (model * vec4(position, 1.0)).xyz;
        gl_Position = projection * lookAt * model * vec4(position, 1.0);
    }
)";

static constexpr const char* BORDER_FRAGMENT_SHADER_SOURCE = R"(
    #version 330 core

    out vec4 fragc;
    in vec3 fragp;

    uniform vec3 cameraDirection;

    void main() {
        fragc = vec4(1.0);
    }
)";





static constexpr const char* COMPOSITE_VERTEX_SHADER_SOURCE = R"(
    #version 330 core
    layout(location = 0) in vec2 position;
    layout(location = 1) in vec2 inUV;
    out vec2 uv;

    void main() {
        uv = inUV;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)";

static constexpr const char* COMPOSITE_FRAGMENT_SHADER_SOURCE = R"(
    #version 330 core
    out vec4 fragc;
    in vec2 uv;

    uniform sampler2D adminBorderTexture;
    uniform sampler2D internBorderTexture;
    uniform sampler2D countryPolygonsTexture;

    void main() {
        vec4 adminBorderColor = texture(adminBorderTexture, uv);
        vec4 internBorderColor = texture(internBorderTexture, uv);
        //vec4 polygonColor = texture(countryPolygonsTexture, uv);

        vec3 background = vec3(16.0, 24.0, 28.0)/255.0;

        vec3 composite = mix(background, vec3(41.0, 124.0, 74.0)/255.0 * 3.0, adminBorderColor.a * 0.2);
        composite = mix(composite, internBorderColor.rgb * vec3(1.0, 0.5, 0.0) * 15.0, internBorderColor.a);
        //composite = mix(composite, polygonColor.rgb * 5.0, polygonColor.a);

        fragc = vec4(composite, 1.0);
    }
)";

#endif //CONFLICT_MAP_SHADERSOURCES_H
