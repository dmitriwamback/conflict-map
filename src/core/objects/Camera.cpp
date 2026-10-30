//
// Created by Dmitri on 2026-10-30.
//

#include "Camera.h"

#include <iostream>
#include <ostream>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {
    position = glm::vec3(0.0f, 0.0f, ZOOM_MAX);
    lookAt = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    yaw = 0.0f;
    pitch = 0.0f;
    distance = ZOOM_MAX;
}

void Camera::Update() {
    pitch = glm::clamp(pitch, -glm::half_pi<float>() + 0.01f, glm::half_pi<float>() - 0.01f
    );

    position.z = distance;
}

glm::mat4 Camera::GetProjectionMatrix(float aspectRatio) {
    return glm::perspective(30.0f * glm::pi<float>()/180.0f, aspectRatio, 0.1f, 2500.0f);
}

glm::mat4 Camera::GetLookAtMatrix() {
    return glm::lookAt(position, position + lookAt, up);
}

glm::vec3 Camera::GetDirection() {
    return position - lookAt;
}

float Camera::GetDistance() {
    return distance;
}

float Camera::GetYaw() {
    return yaw;
}

float Camera::GetPitch() {
    return pitch;
}

void Camera::Move(float deltaX, float deltaY) {

    position.x += deltaX * 500;
    position.y += deltaY * 500;

    Update();
}

void Camera::Zoom(float delta) {
    distance -= delta;
    distance = glm::clamp(distance, ZOOM_MIN, ZOOM_MAX);
}