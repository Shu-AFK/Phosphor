#pragma once

#include "Image.hpp"
#include "Params.hpp"

void run_cpu_pipeline(const Image &src, Image &dst, const Params &params);
