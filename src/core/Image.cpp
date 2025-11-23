#include "Image.hpp"
#include <stdexcept>

Image::Image(int w, int h, int c) : _w(w), _h(h), _c(c) {
  if (w <= 0 || h <= 0 || c <= 0) {
    throw std::invalid_argument("Image dimensions must be positive");
  }

  _pixels.resize(static_cast<size_t>(w) * h * c);
}

int Image::width() const { return _w; }
int Image::height() const { return _h; }
int Image::channels() const { return _c; }
std::size_t Image::size() const { return _pixels.size(); }

uint8_t *Image::data() { return _pixels.data(); }
const uint8_t *Image::data() const { return _pixels.data(); }
