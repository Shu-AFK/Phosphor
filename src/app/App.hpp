#pragma once

#include "AppState.hpp"

#include <glad/glad.h>
#include <nfd.hpp>

#include <GLFW/glfw3.h>

class App {
public:
  App();
  ~App();

  int run();

private:
  GLFWwindow *_window = nullptr;
  AppState _state;
  NFD::Guard _guard;
};
