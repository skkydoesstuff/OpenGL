#include "app/app.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "core/scene/model.hpp"

void App::imguiRender() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    Model* model = scene.getModel("cube");

    ImGui::Begin("Info");
    ImGui::Text("%.1f FPS", this->io.Framerate);
    ImGui::SliderFloat3("Rotation", glm::value_ptr(model->transform.rotation), -360.0f, 360.0f);   
    ImGui::End();

    ImGui::Begin("Shaders");
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
    if (ImGui::BeginTabItem("Outline Settings")) {
        ImGui::Checkbox("Enabled##outline", &this->settings.outlineOn);
        ImGui::SliderFloat("Normal Threshold", &this->settings.normThresh,    0.0f, 1.0f);
        ImGui::SliderFloat("Depth Threshold",  &this->settings.depthThresh,   0.0f, .1f);
        ImGui::SliderFloat("Edge Strength",    &this->settings.edgeStrength,  0.0f, 1.0f);
        ImGui::SliderFloat("Edge Width",       &this->settings.edgeWidth,     0.1f, 5.0f);

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