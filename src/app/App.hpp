#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>

class App {
public:
  App();
  ~App();

  int run();

private:
  GLFWwindow *_window = nullptr;
};
