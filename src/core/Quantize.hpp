#pragma once

#include "Image.hpp"

void quantize_naive(const Image &src, Image &dst, int levelsR, int levelsG,
                    int levelsB);
