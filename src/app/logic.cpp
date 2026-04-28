#include "app/app.hpp"
#include <memory>

#include <GLFW/glfw3.h>

void App::logicLoop() {
    Renderer* r = scene.getRenderer("main");

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

    if (fbWidth != r->getWidth() || fbHeight != r->getHeight()) {
        r->setDimensions(fbWidth, fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);
    }

    r->clearPasses();

    if (this->settings.scanlineOn == true) {
        auto scanline = scene.getPass("scanline");

        scanline->uniforms["uScanlineThickness"] = this->settings.scanThickness;
        scanline->uniforms["uScanlineDarkness"]  = this->settings.scanDarkness;
        scanline->uniforms["uPhosphorStrength"]  = this->settings.phosphorStrength;
        scanline->uniforms["uGlowStrength"]      = this->settings.glowStrength;
        scanline->uniforms["uVignetteStrength"]  = this->settings.vignetteStrength;
        scanline->uniforms["uBrightBoost"]       = this->settings.brightBoost;

        r->addPass(scanline);
    }

    if (this->settings.pixelatedOn == true) {
        auto pixelated = scene.getPass("pixelated");

        pixelated->uniforms["uPixelSize"] = this->settings.pixelSize;
        r->addPass(pixelated);
    }

    Camera* cam = scene.getCamera();
    float speed = 0.05f;
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    // convert rotation (degrees) → forward vector
    float yaw   = glm::radians(cam->rotation.y);
    float pitch = glm::radians(cam->rotation.x);

    forward.x = cos(yaw) * cos(pitch);
    forward.y = sin(pitch);
    forward.z = sin(yaw) * cos(pitch);

    forward = glm::normalize(forward);
    right = glm::normalize(glm::cross(forward, up));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam->position += forward * speed;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam->position -= forward * speed;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam->position -= right * speed;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam->position += right * speed;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cam->position += up * speed;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cam->position -= up * speed;

    static double lastX = 0.0, lastY = 0.0;
    static bool first = true;

    double x, y;
    glfwGetCursorPos(window, &x, &y);

    if (first) {
        lastX = x;
        lastY = y;
        first = false;
    }

    float sensitivity = 0.1f;

    float xoffset = (x - lastX) * sensitivity;
    float yoffset = (lastY - y) * sensitivity;

    lastX = x;
    lastY = y;

    cam->rotation.y += xoffset;
    cam->rotation.x += yoffset;

    // clamp pitch
    if (cam->rotation.x > 89.0f) cam->rotation.x = 89.0f;
    if (cam->rotation.x < -89.0f) cam->rotation.x = -89.0f;
}