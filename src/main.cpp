#include "core/Image.hpp"
#include "core/ImageIO.hpp"
#include "core/Params.hpp"
#include "core/Pipeline.hpp"

int main() {
  Image src = ImageIO::load("../assets/in.jpg");
  Image dst;

  Params params{};
  run_cpu_pipeline(src, dst, params);

  ImageIO::save("../assets/out.jpg", dst);
}
