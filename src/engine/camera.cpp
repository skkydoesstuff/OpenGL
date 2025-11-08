#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include "engine/camera.hpp"
#include "application/constants.hpp"


Camera::Camera(glm::vec3 pos, glm::vec3 upVec, float yawVal, float pitchVal)
    : position(pos), worldUp(upVec), yaw(yawVal), pitch(pitchVal) {
    updateVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjection(float fovDeg, float nearPlane, float farPlane) const {
    return glm::perspective(glm::radians(fovDeg), static_cast<float>(WIDTH) / HEIGHT, nearPlane, farPlane);
}

void Camera::moveForward(float amt) { position += front * amt; }
void Camera::moveRight(float amt)   { position += right * amt; }
void Camera::moveUp(float amt)      { position += up * amt; }

void Camera::rotate(float yawOff, float pitchOff) {
    yaw += yawOff;
    pitch += pitchOff;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    updateVectors();
}

void Camera::updateVectors() {
    glm::vec3 f;
    f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    f.y = sin(glm::radians(pitch));
    f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(f);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
