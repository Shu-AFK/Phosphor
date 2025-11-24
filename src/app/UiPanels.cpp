#include "UiPanels.hpp"

#include "Dialog.hpp"

#include "core/ImageIO.hpp"
#include "util/FileUtils.hpp"
#include "util/VariantUtils.hpp"

#include "imgui.h"

#include <stdexcept>
#include <variant>

void ui::draw_main_section(AppState &state) {}

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

void ui::draw_preview_section(AppState &state) {}

void ui::draw_parameters_section(AppState &state) {}

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

  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(io.DisplaySize);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 6.0f));

  ImGuiWindowFlags windowFlags =
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;

  ImGui::Begin("Phosphor", nullptr, windowFlags);

  ui::draw_main_section(state);
  ui::draw_file_section(state);
  ui::draw_preview_section(state);
  ui::draw_parameters_section(state);
  ui::draw_errors(state);

  ImGui::End();
  ImGui::PopStyleVar(3);
}
