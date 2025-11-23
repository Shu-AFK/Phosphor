#include "ImageIO.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Image ImageIO::load(const std::string &path) {
  int width, height, channels;
  uint8_t *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  if (data == NULL) {
    throw std::runtime_error("Failed to load image: " + path);
  }

  Image img(width, height, channels);
  std::memcpy(img.data(), data, width * height * channels);
  stbi_image_free(data);

  return img;
}

void ImageIO::save(const std::string &path, const Image &img) {
  int r = stbi_write_jpg(path.c_str(), img.width(), img.height(),
                         img.channels(), img.data(), 100);
  if (r == 0) {
    throw std::runtime_error("Failed to save image: " + path);
  }
}
