#pragma once

#include "core/Image.hpp"

#include <glad/glad.h>

class Texture2D {
public:
  Texture2D();
  ~Texture2D();

  void upload_from_image(const Image &img);
  GLuint id() const { return _id; }

private:
  GLuint _id = 0;
  int _width = 0;
  int _height = 0;
  int _channels = 0;
};
