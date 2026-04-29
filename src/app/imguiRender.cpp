#include "app/app.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void App::imguiRender() {
    Camera* cam = scene.getCamera();
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Shaders");
    ImGui::Text("Camera Position: x:%f y:%f z:%f", cam->position.x, cam->position.y, cam->position.z);

    ImGui::BeginTabBar("Shader Options");

    if (ImGui::BeginTabItem("Scanline Settings")) {
        ImGui::Checkbox("Enabled##scanline", &this->settings.scanlineOn);
        ImGui::SliderFloat("Scanline Thickness",  &this->settings.scanThickness,      0.1f, 10.0f);
        ImGui::SliderFloat("Scanline Darkness",   &this->settings.scanDarkness,       0.1f, 10.0f);
        ImGui::SliderFloat("Phosphor Strength",   &this->settings.phosphorStrength,   0.1f, 10.0f);
        ImGui::SliderFloat("Glow Strength",       &this->settings.glowStrength,       0.1f, 10.0f);
        ImGui::SliderFloat("Vignette Strength",   &this->settings.vignetteStrength,   0.1f, 10.0f);
        ImGui::SliderFloat("Bright Boost",        &this->settings.brightBoost,        0.1f, 10.0f);

        ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Pixelated Settings")) {
        ImGui::Checkbox("Enabled##pixelated", &this->settings.pixelatedOn);
        ImGui::SliderFloat("Pixel Size", &this->settings.pixelSize, 1.0f, 32.0f);

        ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
    ImGui::End();

    ImGui::Render();
}