#pragma once

#include "ImageF.hpp"

void quantize_naive(const ImageF &src, ImageF &dst, int levelsR, int levelsG,
                    int levelsB);
