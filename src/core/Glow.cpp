#include "Glow.hpp"
#include "core/ImageF.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

namespace {

inline int clamp_int(int v, int lo, int hi) { return std::min(std::max(v, lo), hi); }

struct GlowWorkspace {
  ImageF exposed;
  ImageF bright;
  ImageF blurTmp;
  ImageF blurOut;

  void ensure(int w, int h, int c) {
    if (!exposed.valid() || exposed.width() != w || exposed.height() != h ||
        exposed.channels() != c) {
      exposed = ImageF(w, h, c);
      bright = ImageF(w, h, c);
      blurTmp = ImageF(w, h, c);
      blurOut = ImageF(w, h, c);
    }
  }
};

// Runs bright-pass in linear space after exposure has been applied to the
// working image.
void bright_pass_filter(const ImageF &src, ImageF &dst, float threshold) {
  int w = src.width();
  int h = src.height();

  float t = std::clamp(threshold, 0.0f, 0.999f);

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      const Vec4f &rgba = src.at(x, y);

      float lum = 0.2126f * rgba.r + 0.7152f * rgba.g + 0.0722f * rgba.b;

      if (lum > t) {
        float scale = (lum - t) / (1.0f - t);
        Vec4f &out = dst.at(x, y);
        out.r = rgba.r * scale;
        out.g = rgba.g * scale;
        out.b = rgba.b * scale;
        out.a = rgba.a;
      } else {
        dst.at(x, y) = Vec4f{0.0f, 0.0f, 0.0f, 0.0f};
      }
    }
  }
}

std::vector<float> gen_gaussian_kernel(int radius) {
  int r = static_cast<int>(std::ceil(radius));
  int size = 2 * r + 1;
  std::vector<float> kernel(size);

  float sigma = radius * 0.5f;
  if (sigma <= 0.0f) {
    sigma = 0.5f;
  }
  float twoSigma2 = 2.0f * sigma * sigma;

  float sum = 0.0f;

  for (int i = 0; i < size; ++i) {
    int x = i - r;
    float w = std::exp(-(x * x) / twoSigma2);
    kernel[i] = w;
    sum += w;
  }

  for (float &w : kernel) {
    w /= sum;
  }

  return kernel;
}

void blur_horizontal(const ImageF &src, ImageF &dst, const std::vector<float> &kernel) {
  int w = src.width();
  int h = src.height();
  int radius = static_cast<int>(kernel.size() / 2);

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

void blur_vertical(const ImageF &src, ImageF &dst, const std::vector<float> &kernel) {
  int w = src.width();
  int h = src.height();
  int radius = static_cast<int>(kernel.size() / 2);

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

void additive_blend(const ImageF &base, const ImageF &glow, ImageF &dst, float intensity) {
  int w = base.width();
  int h = base.height();
  int c = base.channels();

  if (!dst.valid() || dst.width() != w || dst.height() != h || dst.channels() != c) {
    dst = ImageF(w, h, c);
  }

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      const Vec4f &b = base.at(x, y);
      const Vec4f &g = glow.at(x, y);
      Vec4f o;
      o.r = std::clamp(b.r + g.r * intensity, 0.0f, 1.0f);
      o.g = std::clamp(b.g + g.g * intensity, 0.0f, 1.0f);
      o.b = std::clamp(b.b + g.b * intensity, 0.0f, 1.0f);
      o.a = b.a;
      dst.at(x, y) = o;
    }
  }
}

} // namespace

// Glow is executed entirely in 0..1 linear space and additively blended onto the
// base image before gamma encoding back to sRGB.
void apply_glow(const ImageF &src, ImageF &dst, const GlowParams &params) {
  if (!params.enabled || params.intensity <= 0.0f || params.radius <= 0.0f) {
    dst = src;
    return;
  }

  int r = static_cast<int>(std::ceil(params.radius));
  if (r < 1) {
    dst = src;
    return;
  }

  std::vector<float> kernel;
  switch (params.mode) {
  case GlowBlurMode::Gaussian:
  default:
    kernel = gen_gaussian_kernel(r);
    break;
  }

  static GlowWorkspace workspace;
  workspace.ensure(src.width(), src.height(), src.channels());

  // Apply exposure in-place into the working buffer to preserve the base image.
  for (int y = 0; y < src.height(); ++y) {
    for (int x = 0; x < src.width(); ++x) {
      const Vec4f &s = src.at(x, y);
      Vec4f &e = workspace.exposed.at(x, y);
      e.r = s.r * params.exposure;
      e.g = s.g * params.exposure;
      e.b = s.b * params.exposure;
      e.a = s.a;
    }
  }

  bright_pass_filter(workspace.exposed, workspace.bright, params.threshold);
  blur_horizontal(workspace.bright, workspace.blurTmp, kernel);
  blur_vertical(workspace.blurTmp, workspace.blurOut, kernel);

  additive_blend(src, workspace.blurOut, dst, params.intensity);
}
