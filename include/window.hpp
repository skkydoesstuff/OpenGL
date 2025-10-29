#pragma once

#include <GLFW/glfw3.h>

#include <string>

class Window {
private:
  GLFWwindow* window;
  unsigned int width;
  unsigned int height;
  std::string title;  

public:
  Window(const unsigned int width, const unsigned int height, const std::string& title);
  ~Window();

  GLFWwindow* GetWindow() const;
  bool GetWindowShouldClose() const;
  void SetWindowShouldClose();
  void SwapBuffers() const;
};