//
// Created by Dmitri on 2026-10-30.
//

#ifndef CONFLICT_MAP_CAMERA_H
#define CONFLICT_MAP_CAMERA_H


#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>


class Camera {
private:
    glm::vec3 position;
    glm::vec3 lookAt;
    glm::vec3 up;

    float yaw;
    float pitch;
    float distance;

public:
    Camera();

    void Update();

    void Move(float deltaX, float deltaY);
    void Zoom(float delta);

    glm::mat4 GetProjectionMatrix(float aspectRatio);
    glm::mat4 GetLookAtMatrix();
    glm::vec3 GetDirection();
    float GetDistance();
    float GetYaw();
    float GetPitch();

    float ZOOM_MAX = 2000.0f;
    float ZOOM_MIN = 5.0f;
};


#endif //CONFLICT_MAP_CAMERA_H
