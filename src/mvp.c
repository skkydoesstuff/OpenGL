#include <glad/glad.h>
#include <cglm/cglm.h>

#include "mvp.h"
#include "defines.h"

MVP createMVP(vec3 cameraPosition, float fovInRadians, float nearPlane, float farPlane) {
    MVP mvp = {0};

    vec3 cameraTarget = {0.0f, 0.0f, 0.0f};
    vec3 cameraUp = {0.0f, 1.0f, 0.0f};
    glm_lookat(cameraPosition, cameraTarget, cameraUp, mvp.view);

    glm_perspective(fovInRadians, (float)WINDOW_WIDTH / WINDOW_HEIGHT, nearPlane, farPlane, mvp.projection);

    return mvp;
}

void submitMVP(MVP* mvp, unsigned int shaderProgram) {
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, (const float*)mvp->view);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, (const float*)mvp->projection);
}