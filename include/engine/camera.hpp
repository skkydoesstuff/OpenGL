#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "application/constants.hpp"

class Camera {
public:
    Camera(glm::vec3 position = {0.0f, 0.0f, 3.0f},
           glm::vec3 up = {0.0f, 1.0f, 0.0f},
           float yaw = -90.0f, float pitch = 0.0f);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjection(float fovDeg = 45.0f,
                            float nearPlane = NEAR_PLANE, float FAR_PLANE = 100.0f) const;

    void moveForward(float amount);
    void moveRight(float amount);
    void moveUp(float amount);
    void rotate(float yawOffset, float pitchOffset);

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

private:
    void updateVectors();
};
