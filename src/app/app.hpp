
#include <imgui.h>

#include "core/postprocess_pass.hpp"
#include "core/resourceManager.hpp"
#include "core/camera.hpp"
#include "core/texture.hpp"

#include <cstdint>

struct GLFWwindow;

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

    Texture* tex;

    PostProcessPass scanline;
    PostProcessPass outline;
    PostProcessPass pixelated;
    
    bool scanlineOn, outlineOn, pixelatedOn;
    float scanThickness, scanDarkness, phosphorStrength, glowStrength, vignetteStrength, brightBoost;
    float normThresh, depthThresh, edgeStrength, edgeWidth;
    float pixelSize;

    void setup();
    void mainLoop();
    void logicLoop();
    void imguiRender();
    void render();
};