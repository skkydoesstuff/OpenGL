#include <imgui.h>

#include "core/resourceManager.hpp"
#include "core/camera.hpp"

#include <cstdint>

struct GLFWwindow;

struct ShaderSettings {
    bool scanlineOn, outlineOn, pixelatedOn;
    float scanThickness, scanDarkness, phosphorStrength, glowStrength, vignetteStrength, brightBoost;
    float normThresh, depthThresh, edgeStrength, edgeWidth;
    float pixelSize;

    bool bloomOn;
    float thresholdVal, intensity;
    int blurPasses;
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

    ResourceManager rm;
    Camera* cam;

    ShaderSettings settings;

    void setup();
    void mainLoop();
    void logicLoop();
    void imguiRender();
    void render();
};