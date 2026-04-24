#include "app/app.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "core/model.hpp"

void App::imguiRender() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    std::shared_ptr<Model> model = this->rm.models.get("cube");

    ImGui::Begin("Info");
    ImGui::Text("%.1f FPS", this->io.Framerate);
    ImGui::SliderFloat3("Rotation", glm::value_ptr(model->transform.rotation), -360.0f, 360.0f);   
    ImGui::End();

    ImGui::Begin("Shaders");
    ImGui::BeginTabBar("Shader Options");

    if (ImGui::BeginTabItem("Scanline Settings")) {
        ImGui::Checkbox("Enabled##scanline", &this->scanlineOn);
        ImGui::SliderFloat("Scanline Thickness",  &this->scanThickness,      0.1f, 10.0f);
        ImGui::SliderFloat("Scanline Darkness",   &this->scanDarkness,       0.1f, 10.0f);
        ImGui::SliderFloat("Phosphor Strength",   &this->phosphorStrength,   0.1f, 10.0f);
        ImGui::SliderFloat("Glow Strength",       &this->glowStrength,       0.1f, 10.0f);
        ImGui::SliderFloat("Vignette Strength",   &this->vignetteStrength,   0.1f, 10.0f);
        ImGui::SliderFloat("Bright Boost",        &this->brightBoost,        0.1f, 10.0f);

        // Sync to pass every frame
        scanline.uniforms["uScanlineThickness"] = scanThickness;
        scanline.uniforms["uScanlineDarkness"]  = scanDarkness;
        scanline.uniforms["uPhosphorStrength"]  = phosphorStrength;
        scanline.uniforms["uGlowStrength"]      = glowStrength;
        scanline.uniforms["uVignetteStrength"]  = vignetteStrength;
        scanline.uniforms["uBrightBoost"]       = brightBoost;

        ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Outline Settings")) {
        ImGui::Checkbox("Enabled##outline", &this->outlineOn);
        ImGui::SliderFloat("Normal Threshold", &this->normThresh,    0.0f, 1.0f);
        ImGui::SliderFloat("Depth Threshold",  &this->depthThresh,   0.0f, 1.0f);
        ImGui::SliderFloat("Edge Strength",    &this->edgeStrength,  0.0f, 10.0f);
        ImGui::SliderFloat("Edge Width",       &this->edgeWidth,     0.1f, 5.0f);

        outline.uniforms["uNormalThreshold"] = normThresh;
        outline.uniforms["uEdgeStrength"]    = edgeStrength;
        outline.uniforms["uDepthThreshold"]  = depthThresh;
        outline.uniforms["uEdgeWidth"]       = edgeWidth;

        ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Pixelated Settings")) {
        ImGui::Checkbox("Enabled##pixelated", &this->pixelatedOn);
        ImGui::SliderFloat("Pixel Size", &this->pixelSize, 1.0f, 32.0f);

        pixelated.uniforms["uPixelSize"] = pixelSize;

        ImGui::EndTabItem();
    }


    ImGui::EndTabBar();
    ImGui::End();

    ImGui::Render();
}