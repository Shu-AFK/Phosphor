#pragma once

#include "Image.hpp"

#include <string>

class ImageIO {
public:
  static Image load(const std::string &path);
  static void save(const std::string &path, const Image &img);
};
