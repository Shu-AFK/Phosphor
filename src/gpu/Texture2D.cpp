#include "Texture2D.hpp"
#include <GL/gl.h>

Texture2D::Texture2D() { glGenTextures(1, &_id); }

Texture2D::~Texture2D() {
  if (_id != 0) {
    glDeleteTextures(1, &_id);
  }
}

void Texture2D::upload_from_image(const Image &img) {
  _width = img.width();
  _height = img.height();
  _channels = img.channels();

  glBindTexture(GL_TEXTURE_2D, _id);

  GLenum format = (_channels == 3) ? GL_RGB : GL_RGBA;

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format,
               GL_UNSIGNED_BYTE, img.data());
}
