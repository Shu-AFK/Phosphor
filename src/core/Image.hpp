#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

class Image {
public:
  Image() = default;

  // This automatically resizes the vector to the correct size
  Image(int w, int h, int c);

  int width() const;
  int height() const;
  int channels() const;
  std::size_t size() const;

  const uint8_t *data() const;
  uint8_t *data();

private:
  int _w = 0;
  int _h = 0;
  int _c = 0;
  std::vector<uint8_t> _pixels;
};
