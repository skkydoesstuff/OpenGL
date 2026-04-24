
#include <imgui.h>

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

    void setup();
    void mainLoop();
    void imguiRender();
    void render();
};