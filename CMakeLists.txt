cmake_minimum_required(VERSION 4.3)
project(conflict_map)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 REQUIRED COMPONENTS
        Widgets
        OpenGL
        OpenGLWidgets
        Network
)
find_package(glm REQUIRED)

add_executable(conflict_map main.cpp
        src/widgets/MapRenderer.cpp
        src/widgets/MapRenderer.h
        src/util/ACLED.cpp
        src/util/ACLED.h
        src/core/framebuffer/ColorFramebuffer.cpp
        src/core/framebuffer/ColorFramebuffer.h
        src/core/framebuffer/DepthFramebuffer.cpp
        src/core/framebuffer/DepthFramebuffer.h
        src/core/framebuffer/Quad.cpp
        src/core/framebuffer/Quad.h
        src/core/objects/Borders.cpp
        src/core/objects/Borders.h
        src/core/objects/Camera.cpp
        src/core/objects/Camera.h
        src/core/shader/Shader.cpp
        src/core/shader/Shader.h
        src/core/shader/ShaderSources.h
        src/util/UCDP.cpp
        src/util/UCDP.h
        src/core/objects/Marker.cpp
        src/core/objects/Marker.h
        src/util/Math.h)

target_link_libraries(conflict_map PRIVATE
        Qt6::Widgets
        Qt6::OpenGL
        Qt6::OpenGLWidgets
        Qt6::Network
        glm::glm
)