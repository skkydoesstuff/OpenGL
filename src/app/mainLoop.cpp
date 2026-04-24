#include "app/app.hpp"

#include <GLFW/glfw3.h>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void App::mainLoop() {
    while (!glfwWindowShouldClose(this->window)) {
        //this->logicLoop();
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);
        }

        this->logicLoop();

        this->imguiRender();

        this->render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}