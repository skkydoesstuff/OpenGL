#include "core/scene/objects/camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float fovInRadians, float aspectRatio, float zNear, float zFar) {
    this->projection = glm::perspective(glm::radians(fovInRadians), aspectRatio, zNear, zFar);
}

void Camera::setProjection(float fovInRadians, float aspectRatio, float zNear, float zFar) {
    this->projection = glm::perspective(glm::radians(fovInRadians), aspectRatio, zNear, zFar);
}

glm::mat4 Camera::getView() {
    float yaw = glm::radians(rotation.y);
    float pitch = glm::radians(rotation.x);

    glm::vec3 forward;
    forward.x = cos(yaw) * cos(pitch);
    forward.y = sin(pitch);
    forward.z = sin(yaw) * cos(pitch);

    return glm::lookAt(
        position,
        position + glm::normalize(forward),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
}

glm::mat4 Camera::getProjection() {
    return this->projection;
}