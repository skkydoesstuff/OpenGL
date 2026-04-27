#include "app/app.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <utils/stb_image.h>


void App::render() {
    Renderer* r = scene.getRenderer("main");

    r->beginScene();

    scene.drawScene("base");
    
    r->endScene();
}