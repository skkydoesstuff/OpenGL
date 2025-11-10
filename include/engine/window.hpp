#pragma once

#include <GLFW/glfw3.h>

class Window {
private:
  GLFWwindow* window;

  double lastTime;
  double deltaTime;

public:
  Window(unsigned int WIDTH, unsigned int HEIGHT, const char* TITLE);
  ~Window();
  bool shouldClose();
  void update();

  double getDeltaTime();
};