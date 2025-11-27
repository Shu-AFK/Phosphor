#pragma once

#include "ImageF.hpp"
#include "Params.hpp"

float quantize_channel_uniform(float value, int levels);

Vec3f quantize_color(const Vec3f &src, QuantizeMode mode, int levelsR,
                     int levelsG, int levelsB);

void quantize_naive(const ImageF &src, ImageF &dst, QuantizeMode mode,
                    int levelsR, int levelsG, int levelsB);
