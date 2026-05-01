#pragma once

#include <glm/glm.hpp>

class Camera {
public:
    Camera(float fovInRadians, float aspectRatio, float zNear, float zFar);
    ~Camera() = default;

    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    void setProjection(float fovInRadians, float aspectRatio, float zNear, float zFar);

    glm::mat4 getView();
    glm::mat4 getProjection();

    glm::vec3 position{0};
    glm::vec3 rotation{0, -90, 0};

private:
    glm::mat4 view{1.0f};
    glm::mat4 projection{0};
};