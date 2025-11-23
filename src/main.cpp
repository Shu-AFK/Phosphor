#include "core/Image.hpp"
#include "core/ImageIO.hpp"
#include "core/Quantize.hpp"

int main() {
  Image src = ImageIO::load("../assets/in.jpg");
  Image dst;
  quantize_naive(src, dst, 8, 8, 8);
  ImageIO::save("../assets/out.jpg", dst);
}
