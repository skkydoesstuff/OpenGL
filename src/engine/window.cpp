#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/window.hpp"

#include <iostream>

Window::Window(unsigned int WIDTH, unsigned int HEIGHT, const char* TITLE) {
  if (!glfwInit()) {
      std::cout << "Failed to initialize GLFW!" << std::endl;
      exit(-1);
  }
  
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
  if (!window) {
      std::cout << "Failed to create the GLFW Window!" << std::endl;
      glfwTerminate();
      exit(-1);
  }
  
  glfwMakeContextCurrent(window);
  
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
      std::cout << "Failed to load OpenGL with GLAD!" << std::endl;
      glfwTerminate();
      exit(-1);
  }

  glEnable(GL_DEPTH_TEST);
  glfwSwapInterval(1);

  lastTime = glfwGetTime();
}

Window::~Window() {
  if (window) {
      glfwDestroyWindow(window);
  }
  glfwTerminate();
}

bool Window::shouldClose() {
  return glfwWindowShouldClose(window);
}

void Window::update() {
  double currentTime = glfwGetTime();
  deltaTime = currentTime - lastTime;
  lastTime = currentTime;

  glfwPollEvents();
  glfwSwapBuffers(window);
}

double Window::getDeltaTime() {
  return deltaTime;
}