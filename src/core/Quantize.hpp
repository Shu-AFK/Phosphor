#pragma once

#include "ImageF.hpp"
enum class QuantizeMode {
  None,
  UniformPerChannel,
};

struct QuantizeParams {
  QuantizeMode mode = QuantizeMode::None;
  int levelsR = 4;
  int levelsG = 4;
  int levelsB = 4;
};

float quantize_channel_uniform(float value, int levels);

Vec3f quantize_color(const Vec3f &src, QuantizeMode mode, int levelsR,
                     int levelsG, int levelsB);

void quantize_naive(const ImageF &src, ImageF &dst, QuantizeMode mode,
                    int levelsR, int levelsG, int levelsB);
