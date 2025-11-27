#pragma once

#include "ImageF.hpp"

void quantize_ordered_dither(const ImageF &src, ImageF &dst, int levelsR,
                             int levelsG, int levelsB, float ditherStrength);
