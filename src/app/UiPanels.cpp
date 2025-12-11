#include "UiPanels.hpp"

#include "Dialog.hpp"

#include "core/ColorScheme.hpp"
#include "core/Glow.hpp"
#include "core/ImageIO.hpp"
#include "core/Params.hpp"
#include "core/Pipeline.hpp"
#include "util/FileUtils.hpp"
#include "util/VariantUtils.hpp"

#include "imgui.h"
#include <GLFW/glfw3.h>

#include <algorithm>
#include <stdexcept>
#include <variant>

namespace {
bool SliderFloatReprocess(const char *label, float *v, float v_min, float v_max,
                          AppState &state) {
  ImGui::SliderFloat(label, v, v_min, v_max);

  if (ImGui::IsItemDeactivatedAfterEdit()) {
    state.needsReprocess = true;
    return true;
  }
  return false;
}

bool SliderIntReprocess(const char *label, int *v, int v_min, int v_max,
                        AppState &state) {
  ImGui::SliderInt(label, v, v_min, v_max);

  if (ImGui::IsItemDeactivatedAfterEdit()) {
    state.needsReprocess = true;
    return true;
  }
  return false;
}

void draw_color_section(AppState &state) {
  ImGui::Text("Color");

  SliderFloatReprocess("Gamma In", &state.params.gammaIn, 1.0f, 3.0f, state);
  SliderFloatReprocess("Gamma Out", &state.params.gammaOut, 1.0f, 3.0f, state);
}

void draw_color_scheme_section(AppState &state) {
  ImGui::Text("Color Scheme");

  if (ImGui::Checkbox("Enable Color Scheme", &state.params.colorScheme.enabled)) {
    state.needsReprocess = true;
  }

  if (state.params.colorScheme.enabled) {
    if (ImGui::Checkbox("Use Custom Color", &state.params.colorScheme.useCustom)) {
      state.needsReprocess = true;
    }

    if (state.params.colorScheme.useCustom) {
      float color[3] = {state.params.colorScheme.customColor.r,
                        state.params.colorScheme.customColor.g,
                        state.params.colorScheme.customColor.b};
      if (ImGui::ColorEdit3("Custom Color", color)) {
        state.params.colorScheme.customColor.r = color[0];
        state.params.colorScheme.customColor.g = color[1];
        state.params.colorScheme.customColor.b = color[2];
        state.needsReprocess = true;
      }
    } else {
      if (ImGui::BeginCombo("Preset",
                            COLOR_PRESETS[state.params.colorScheme.presetIndex].name)) {
        for (int i = 0; i < COLOR_PRESET_COUNT; ++i) {
          bool isSelected = (state.params.colorScheme.presetIndex == i);
          if (ImGui::Selectable(COLOR_PRESETS[i].name, isSelected)) {
            state.params.colorScheme.presetIndex = i;
            state.needsReprocess = true;
          }
          if (isSelected) {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
    }
  }
}

void draw_glow_section(AppState &state) {
  ImGui::Text("Glow");

  if (ImGui::Checkbox("Enable Glow", &state.params.glow.enabled)) {
    state.needsReprocess = true;
  }

  if (state.params.glow.enabled) {
    static const char *blurNames[] = {"Gaussian", "Box"};
    int blurMode = (int)state.params.glow.mode;
    if (ImGui::Combo("Blur Mode", &blurMode, blurNames,
                     IM_ARRAYSIZE(blurNames))) {
      state.params.glow.mode = (GlowBlurMode)blurMode;
      state.needsReprocess = true;
    }

    SliderFloatReprocess("Exposure", &state.params.glow.exposure, 0.1f, 2.0f,
                         state);
    SliderFloatReprocess("Threshold", &state.params.glow.threshold, 0.0f, 1.0f,
                         state);
    SliderFloatReprocess("Radius", &state.params.glow.radius, 1.0f, 40.0f, state);
    SliderFloatReprocess("Intensity##glow", &state.params.glow.intensity, 0.0f,
                         4.0f, state);
  }
}

void draw_quantization_section(AppState &state) {
  ImGui::Text("Quantization");

  if (ImGui::Checkbox("Enable Quantization", &state.params.quantize.enabled)) {
    state.needsReprocess = true;
  }

  if (state.params.quantize.enabled) {
    bool wasLocked = state.lockChannels;

    if (state.lockChannels) {
      if (SliderIntReprocess("Levels RGB", &state.params.quantize.levelsR, 1, 32,
                             state)) {
        int shared = state.params.quantize.levelsR;
        state.params.quantize.levelsG = shared;
        state.params.quantize.levelsB = shared;
      }
    } else {
      SliderIntReprocess("Levels R", &state.params.quantize.levelsR, 1, 32,
                         state);
      SliderIntReprocess("Levels G", &state.params.quantize.levelsG, 1, 32,
                         state);
      SliderIntReprocess("Levels B", &state.params.quantize.levelsB, 1, 32,
                         state);
    }

    ImGui::Checkbox("Lock Channels", &state.lockChannels);

    if (!wasLocked && state.lockChannels) {
      int shared = state.params.quantize.levelsR;
      state.params.quantize.levelsG = shared;
      state.params.quantize.levelsB = shared;
      state.needsReprocess = true;
    }
  }
}

void draw_dithering_section(AppState &state) {
  ImGui::Text("Dithering");

  if (ImGui::Checkbox("Enable Dithering", &state.params.dither.enabled)) {
    state.needsReprocess = true;

    if (state.params.dither.enabled && !state.params.quantize.enabled) {
      state.errors.push("You need to enable quantization for dithering!");
    }
  }

  if (state.params.dither.enabled) {
    SliderFloatReprocess("Dither Strength", &state.params.dither.strength, 0.0f,
                         2.0f, state);
  }
}

void draw_scanlines_section(AppState &state) {
  ImGui::Text("Scanlines");

  if (ImGui::Checkbox("Enable Scanlines", &state.params.scanlines.enabled)) {
    state.needsReprocess = true;
  }

  if (state.params.scanlines.enabled) {
    SliderIntReprocess("Thickness", &state.params.scanlines.thickness, 1, 30,
                       state);
    SliderIntReprocess("Spacing", &state.params.scanlines.spacing, 1, 5, state);
    SliderFloatReprocess("Gap Darkness", &state.params.scanlines.gapDarkness,
                         0.0f, 1.0f, state);
  }
}
} // namespace

void ui::draw_main_section(AppState &state) {
  if (ImGui::BeginTable("MainSplit", 2, ImGuiTableFlags_SizingStretchProp)) {
    // 70% preview 30% controls
    ImGui::TableSetupColumn("Preview", ImGuiTableColumnFlags_WidthStretch,
                            0.7f);
    ImGui::TableSetupColumn("Controls", ImGuiTableColumnFlags_WidthStretch,
                            0.3f);
    ImGui::TableNextRow();

    ImGui::TableSetColumnIndex(0);
    ui::draw_preview_section(state);

    ImGui::TableSetColumnIndex(1);
    ui::draw_parameters_section(state);

    ImGui::EndTable();
  }
}

void ui::draw_file_section(AppState &state) {
  if (ImGui::BeginTable("filetable", 2, ImGuiTableFlags_SizingStretchProp)) {

    ImGui::TableNextColumn();
    ImGui::Text("Filename: %s", state.fileName.c_str());

    ImGui::TableNextColumn();

    float buttonWidth = 80.f;
    float spacing = ImGui::GetStyle().ItemSpacing.x;
    float total = buttonWidth * 2.f + spacing;
    float avail = ImGui::GetContentRegionAvail().x;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (avail - total));

    if (ImGui::Button("Load", ImVec2(buttonWidth, 0))) {
      auto result = Dialog::open_file_dialog();
      std::visit(overloaded{[&](const std::string &path) {
                              state.currentPath = path;
                              state.fileName = util::filename_from_path(path);
                              try {
                                state.original = ImageIO::load(path);
                                state.processed = state.original;
                                state.imageLoaded = true;
                                state.needsReprocess = true;
                              } catch (std::runtime_error &e) {
                                state.errors.push(e.what());
                              }
                            },
                            [](const Dialog::FileDialogCancelled &) {},
                            [&](const Dialog::FileDialogError &e) {
                              state.errors.push(e.message);
                            }},
                 result);
    }

    ImGui::SameLine();

    if (ImGui::Button("Save", ImVec2(buttonWidth, 0))) {
      if (state.imageLoaded) {
        auto result = Dialog::save_file_dialog();
        std::visit(overloaded{[&](const std::string &savePath) {
                                try {
                                  ImageIO::save(savePath, state.processed);
                                } catch (std::runtime_error &e) {
                                  state.errors.push(e.what());
                                }
                              },
                              [](const Dialog::FileDialogCancelled &) {},
                              [&](const Dialog::FileDialogError &e) {
                                state.errors.push(e.message);
                              }},
                   result);
      }
    }

    ImGui::EndTable();
  }
}

void ui::draw_preview_section(AppState &state) {
  ImGui::BeginChild("Preview Panel");

  if (!state.imageLoaded) {
    ImGui::Text("No image loaded");
    ImGui::EndChild();
    return;
  }

  if (state.needsReprocess) {
    run_cpu_pipeline(state.original, state.processed, state.params);

    state.previewTexture->upload_from_image(state.processed);
    state.needsReprocess = false;
  }

  float pW = state.processed.width();
  float pH = state.processed.height();
  ImVec2 avail = ImGui::GetContentRegionAvail();

  float baseScale = std::min(avail.x / pW, avail.y / pH);
  ImVec2 drawSize(pW * baseScale, pH * baseScale);

  static float zoom_factor = 1.0f;
  static ImVec2 uvCenter = ImVec2(0.5f, 0.5f);

  ImGui::Text("Zoom");
  ImGui::SameLine();
  if (ImGui::Button("In")) {
    zoom_factor *= 1.1f;
    zoom_factor = std::clamp(zoom_factor, 1.0f, 5.0f);
  }
  ImGui::SameLine();
  if (ImGui::Button("Out")) {
    zoom_factor /= 1.1f;
    zoom_factor = std::clamp(zoom_factor, 1.0f, 5.0f);
    if (zoom_factor == 1.0f)
      uvCenter = ImVec2(0.5f, 0.5f);
  }

  zoom_factor = std::clamp(zoom_factor, 1.0f, 5.0f);

  float halfSpan = 0.5f / zoom_factor;
  ImVec2 uv0(uvCenter.x - halfSpan, uvCenter.y - halfSpan);
  ImVec2 uv1(uvCenter.x + halfSpan, uvCenter.y + halfSpan);

  uv0.x = std::clamp(uv0.x, 0.0f, 1.0f);
  uv0.y = std::clamp(uv0.y, 0.0f, 1.0f);
  uv1.x = std::clamp(uv1.x, 0.0f, 1.0f);
  uv1.y = std::clamp(uv1.y, 0.0f, 1.0f);

  ImVec2 imagePos = ImGui::GetCursorScreenPos();
  ImGui::Image((ImTextureID)state.previewTexture->id(), drawSize, uv0, uv1);

  ImGuiIO &io = ImGui::GetIO();
  bool ctrl = io.KeyCtrl;

  bool plusPressed = ImGui::IsKeyPressed(ImGuiKey_Equal) ||
                     ImGui::IsKeyPressed(ImGuiKey_KeypadAdd);

  if (ImGui::IsItemHovered() && ctrl && plusPressed) {
    ImVec2 mouse = ImGui::GetMousePos();
    ImVec2 local((mouse.x - imagePos.x) / drawSize.x,
                 (mouse.y - imagePos.y) / drawSize.y);

    local.x = std::clamp(local.x, 0.0f, 1.0f);
    local.y = std::clamp(local.y, 0.0f, 1.0f);

    ImVec2 currentUv0 = uv0;
    ImVec2 currentUv1 = uv1;
    ImVec2 uvUnderMouse(currentUv0.x + local.x * (currentUv1.x - currentUv0.x),
                        currentUv0.y + local.y * (currentUv1.y - currentUv0.y));

    zoom_factor *= 1.1f;
    zoom_factor = std::clamp(zoom_factor, 1.0f, 5.0f);
    uvCenter = uvUnderMouse;
  }

  ImGui::EndChild();
}

void ui::draw_parameters_section(AppState &state) {
  ImGui::BeginChild("Control Panel");

  draw_color_section(state);
  ImGui::Separator();

  draw_color_scheme_section(state);
  ImGui::Separator();

  draw_glow_section(state);
  ImGui::Separator();

  draw_quantization_section(state);
  ImGui::Separator();

  draw_dithering_section(state);
  ImGui::Separator();

  draw_scanlines_section(state);

  ImGui::EndChild();
}

void ui::draw_errors(AppState &state) {
  if (state.errors.should_open_popup()) {
    ImGui::OpenPopup("Error");
  }

  if (ImGui::BeginPopupModal("Error", nullptr,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::TextWrapped("Error occured: %s", state.errors.current().c_str());
    ImGui::Separator();

    if (ImGui::Button("OK")) {
      state.errors.pop();
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}

void ui::draw_window(AppState &state) {
  ImGuiIO &io = ImGui::GetIO();

  int fbW, fbH;
  glfwGetFramebufferSize(glfwGetCurrentContext(), &fbW, &fbH);

  static int lastW = 0, lastH = 0;

  if ((fbW != lastW || fbH != lastH) && state.imageLoaded) {
    state.needsReprocess = true;
  }

  lastW = fbW;
  lastH = fbH;

  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(io.DisplaySize);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 6.0f));

  ImGuiWindowFlags windowFlags =
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;

  ImGui::Begin("Phosphor", nullptr, windowFlags);

  ui::draw_file_section(state);
  ui::draw_main_section(state);
  ui::draw_errors(state);

  ImGui::End();
  ImGui::PopStyleVar(3);
}
