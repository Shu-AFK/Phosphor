#pragma once

#include "ImageF.hpp"

struct ColorPresetDef {
  const char *name;
  Vec3f color;
};

constexpr ColorPresetDef COLOR_PRESETS[] = {
    // Pure primaries
    {"Red", {1.0f, 0.0f, 0.0f}},
    {"Green", {0.0f, 1.0f, 0.0f}},
    {"Blue", {0.0f, 0.0f, 1.0f}},

    // Pure secondaries
    {"Cyan", {0.0f, 1.0f, 1.0f}},
    {"Magenta", {1.0f, 0.0f, 1.0f}},
    {"Yellow", {1.0f, 1.0f, 0.0f}},

    // Retro monitor phosphors
    {"Amber", {1.0f, 0.75f, 0.0f}},
    {"P1 Green", {0.2f, 1.0f, 0.2f}},
    {"P3 Amber", {1.0f, 0.78f, 0.25f}},

    // Stylized
    {"Hot Pink", {1.0f, 0.4f, 0.7f}},
    {"Synthwave Purple", {0.7f, 0.3f, 1.0f}},
    {"Neon Orange", {1.0f, 0.5f, 0.0f}},
    {"Ice Blue", {0.5f, 0.8f, 1.0f}},
    {"Lime", {0.75f, 1.0f, 0.0f}},
    {"Coral", {1.0f, 0.5f, 0.5f}},

    // Grayscale
    {"White", {1.0f, 1.0f, 1.0f}},
};

constexpr int COLOR_PRESET_COUNT =
    sizeof(COLOR_PRESETS) / sizeof(COLOR_PRESETS[0]);

struct ColorSchemeParams {
  bool enabled = false;
  int presetIndex = 0;
  Vec3f customColor = {1.0f, 1.0f, 1.0f};
  bool useCustom = false;
};

void apply_color_scheme(const ImageF &src, ImageF &dst,
                        const ColorSchemeParams &params);
