#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.hpp"
#include "defines.hpp"

#include <iostream>
#include <string>

Window::Window(const unsigned int width, const unsigned int height, const std::string& title) {
  glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	std::cout << "OpenGL Version: " << OPENGL_VERSION_MAJOR << '.' << OPENGL_VERSION_MINOR << std::endl;

	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);
}

Window::~Window() {
    if (window)
        glfwDestroyWindow(window);
    glfwTerminate();
}

GLFWwindow* Window::GetWindow() const {
  return window;
}

bool Window::GetWindowShouldClose() const {
  return glfwWindowShouldClose(window);
}

void Window::SetWindowShouldClose() {
  glfwSetWindowShouldClose(window, true);
}

void Window::SwapBuffers() const {
  glfwSwapBuffers(window);
}