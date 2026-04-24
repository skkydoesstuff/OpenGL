#include "app/app.hpp"

#include <glm/gtc/type_ptr.hpp>

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

    ImGui::Render();
}