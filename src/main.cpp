#include "core/Dither.hpp"
#include "core/Image.hpp"
#include "core/ImageIO.hpp"
#include "core/Quantize.hpp"

int main() {
  Image src = ImageIO::load("../assets/in.jpg");
  Image dst;
  quantize_ordered_dither(src, dst, 8, 8, 8, 2.0f);
  ImageIO::save("../assets/out.jpg", dst);
}
