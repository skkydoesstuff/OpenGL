#include "app/app.hpp"

#include "GLFW/glfw3.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

App::~App() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}