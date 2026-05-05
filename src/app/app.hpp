#include <imgui.h>

#include "core/scene/scene.hpp"

#include <cstdint>

struct GLFWwindow;

struct ShaderSettings {
    bool scanlineOn, pixelatedOn;
    float scanThickness, scanDarkness, phosphorStrength, glowStrength, vignetteStrength, brightBoost;
    float pixelSize;
};

class App {
public:
    const uint32_t width = 1280;
    const uint32_t height = 720;

    void run();

    App();
    ~App();

private:
    GLFWwindow* window;
    ImGuiIO io;

    std::unique_ptr<Scene> scene;
    ShaderSettings settings;

    bool mouseLocked = true;
    bool firstMouse = true;

    void setup();
    void mainLoop();
    void logicLoop();
    void imguiRender();
    void render();
};
