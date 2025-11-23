#include "Pipeline.hpp"
#include "Dither.hpp"
#include "Quantize.hpp"

void run_cpu_pipeline(const Image &src, Image &dst, const Params &params) {
  switch (params.ditherMode) {
  case DitherMode::None:
    quantize_naive(src, dst, params.levelsR, params.levelsG, params.levelsB);
    break;

  case DitherMode::Ordered4x4:
    quantize_ordered_dither(src, dst, params.levelsR, params.levelsG,
                            params.levelsB, params.ditherStrength);
    break;
  }
}
