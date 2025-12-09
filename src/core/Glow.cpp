#include "Glow.hpp"
#include "core/ImageF.hpp"

#include <cmath>
#include <vector>

#define GAMMA_IN 2.2
#define PI 3.1415

namespace {

inline int clamp_int(int v, int lo, int hi) {
  return v < lo ? lo : (v > hi ? hi : v);
}

void exposure_scaling(const ImageF &src, ImageF &dst, float exposure) {
  int w = src.width();
  int h = src.height();
  int c = src.channels();

  dst = ImageF(w, h, c);

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {

      auto rgba = src.at(x, y);

      for (int ch = 0; ch < c; ++ch) {
        float v = rgba[ch] / 255.0f;
        float lin = std::pow(v, 1.0f / GAMMA_IN);
        lin *= exposure;
        rgba[ch] = lin * 255.0f;
      }

      dst.at(x, y) = rgba;
    }
  }
}

void bright_pass_filter(const ImageF &src, ImageF &dst, float threshold) {
  int w = src.width();
  int h = src.height();
  int c = src.channels();

  dst = ImageF(w, h, c);

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      const Vec4f &rgba = src.at(x, y);

      float lum = 0.2126f * rgba.r + 0.7152f * rgba.g + 0.0722f * rgba.b;
      lum /= 255.0f;

      if (lum > threshold) {
        float scale = (lum - threshold) / (1.0f - threshold);
        dst.at(x, y) = rgba * scale;
      } else {
        dst.at(x, y) = Vec4f{0, 0, 0, 0};
      }
    }
  }
}

std::vector<float> gen_gaussian_kernel(int radius) {
  int size = 2 * radius + 1;
  std::vector<float> kernel(size);

  float sigma = radius * 0.5f;
  float twoSigma2 = 2.0f * sigma * sigma;
  float invSigmaRoot = 1.0f / (std::sqrt(2.0f * PI) * sigma);

  float sum = 0.0f;

  for (int i = 0; i < size; ++i) {
    int x = i - radius;
    float w = invSigmaRoot * std::exp(-(x * x) / twoSigma2);
    kernel[i] = w;
    sum += w;
  }

  for (float &w : kernel)
    w /= sum;

  return kernel;
}

void blur_horizontal(const ImageF &src, ImageF &dst,
                     const std::vector<float> &kernel) {
  int w = src.width();
  int h = src.height();
  int c = src.channels();
  int radius = static_cast<int>(kernel.size() / 2);

  dst = ImageF(w, h, c);

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      Vec4f acc{0.0f, 0.0f, 0.0f, 0.0f};

      for (int k = -radius; k <= radius; ++k) {
        int sx = clamp_int(x + k, 0, w - 1);
        float w_k = kernel[k + radius];

        const Vec4f &sample = src.at(sx, y);
        acc += sample * w_k;
      }

      dst.at(x, y) = acc;
    }
  }
}

void blur_vertical(const ImageF &src, ImageF &dst,
                   const std::vector<float> &kernel) {
  int w = src.width();
  int h = src.height();
  int c = src.channels();
  int radius = static_cast<int>(kernel.size() / 2);

  dst = ImageF(w, h, c);

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      Vec4f acc{0.0f, 0.0f, 0.0f, 0.0f};

      for (int k = -radius; k <= radius; ++k) {
        int sy = clamp_int(y + k, 0, h - 1);
        float w_k = kernel[k + radius];

        const Vec4f &sample = src.at(x, sy);
        acc += sample * w_k;
      }

      dst.at(x, y) = acc;
    }
  }
}

} // namespace

void apply_glow(const ImageF &src, ImageF &dst, const GlowParams &params) {
  exposure_scaling(src, dst, params.exposure);

  ImageF tmp = dst;
  bright_pass_filter(tmp, dst, params.threshold);

  std::vector<float> kernel = gen_gaussian_kernel(ceil(params.radius));

  ImageF blur_tmp;
  blur_horizontal(dst, blur_tmp, kernel);
  blur_vertical(blur_tmp, dst, kernel);
}
