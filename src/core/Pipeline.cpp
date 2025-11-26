#include "Pipeline.hpp"
#include "Dither.hpp"
#include "Filter.hpp"
#include "Quantize.hpp"

#include <cassert>

void run_cpu_pipeline(const Image &src, Image &dst, const Params &params) {
  dst = src;

  switch (params.filterMode) {
  case FilterMode::None:
    break;
  case FilterMode::Grayscale:
  case FilterMode::RedChannel:
  case FilterMode::GreenChannel:
  case FilterMode::BlueChannel:
    apply_channel_filter(src, dst, params.filterMode, params.channelIntensity);
    break;

  default:
    assert(false && "Invalid filter mode");
    break;
  }

  Image tmp = dst;
  switch (params.ditherMode) {
  case DitherMode::None:
    quantize_naive(tmp, dst, params.levelsR, params.levelsG, params.levelsB);
    break;

  case DitherMode::Ordered4x4:
    quantize_ordered_dither(tmp, dst, params.levelsR, params.levelsG,
                            params.levelsB, params.ditherStrength);
    break;

  default:
    assert(false && "Invalid dither mode!");
    break;
  }
}
