#include "Style.hpp"

#include <imgui.h>

#include <filesystem>
#include <string>

namespace ui {
namespace {
constexpr ImVec4 color_from_hex(float r, float g, float b, float a = 1.0f) {
  return ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, a);
}
} // namespace

void load_fonts() {
  ImGuiIO &io = ImGui::GetIO();
  const std::string fontPath = "../assets/Inter_18pt-Medium.ttf";
  constexpr float fontSize = 17.0f;

  io.Fonts->Clear();

  if (std::filesystem::exists(fontPath)) {
    ImFontConfig config;
    config.OversampleH = 3;
    config.OversampleV = 2;
    config.GlyphOffset = ImVec2(0.0f, -0.5f);
    io.FontDefault =
        io.Fonts->AddFontFromFileTTF(fontPath.c_str(), fontSize, &config);
  }

  if (!io.FontDefault) {
    io.FontDefault = io.Fonts->AddFontDefault();
  }
}

void apply_modern_style() {
  ImGuiStyle &style = ImGui::GetStyle();

  style.WindowPadding = ImVec2(12.0f, 10.0f);
  style.FramePadding = ImVec2(10.0f, 8.0f);
  style.CellPadding = ImVec2(8.0f, 6.0f);
  style.ItemSpacing = ImVec2(10.0f, 10.0f);
  style.ItemInnerSpacing = ImVec2(8.0f, 6.0f);

  style.WindowBorderSize = 1.0f;
  style.FrameBorderSize = 0.0f;
  style.PopupBorderSize = 1.0f;

  style.WindowRounding = 12.0f;
  style.ChildRounding = 12.0f;
  style.FrameRounding = 10.0f;
  style.PopupRounding = 10.0f;
  style.ScrollbarRounding = 12.0f;
  style.GrabRounding = 12.0f;
  style.TabRounding = 8.0f;

  ImVec4 *colors = style.Colors;
  colors[ImGuiCol_Text] = color_from_hex(236, 232, 247);
  colors[ImGuiCol_TextDisabled] = color_from_hex(120, 103, 142);
  colors[ImGuiCol_WindowBg] = color_from_hex(8, 6, 16, 0.97f);
  colors[ImGuiCol_ChildBg] = color_from_hex(18, 14, 30, 0.7f);
  colors[ImGuiCol_PopupBg] = color_from_hex(24, 18, 38, 0.95f);
  colors[ImGuiCol_Border] = color_from_hex(82, 55, 145, 0.35f);
  colors[ImGuiCol_BorderShadow] = color_from_hex(0, 0, 0, 0.0f);
  colors[ImGuiCol_FrameBg] = color_from_hex(36, 24, 58, 0.85f);
  colors[ImGuiCol_FrameBgHovered] = color_from_hex(104, 75, 177, 0.9f);
  colors[ImGuiCol_FrameBgActive] = color_from_hex(140, 105, 223, 1.0f);
  colors[ImGuiCol_TitleBg] = color_from_hex(18, 12, 30);
  colors[ImGuiCol_TitleBgActive] = color_from_hex(56, 33, 92);
  colors[ImGuiCol_TitleBgCollapsed] = color_from_hex(0, 0, 0, 0.5f);
  colors[ImGuiCol_MenuBarBg] = color_from_hex(16, 12, 28);
  colors[ImGuiCol_ScrollbarBg] = color_from_hex(12, 8, 20, 0.6f);
  colors[ImGuiCol_ScrollbarGrab] = color_from_hex(70, 50, 116, 0.9f);
  colors[ImGuiCol_ScrollbarGrabHovered] = color_from_hex(116, 86, 172, 1.0f);
  colors[ImGuiCol_ScrollbarGrabActive] = color_from_hex(149, 114, 215, 1.0f);
  colors[ImGuiCol_CheckMark] = color_from_hex(198, 162, 255, 1.0f);
  colors[ImGuiCol_SliderGrab] = color_from_hex(149, 114, 215, 1.0f);
  colors[ImGuiCol_SliderGrabActive] = color_from_hex(198, 162, 255, 1.0f);
  colors[ImGuiCol_Button] = color_from_hex(70, 50, 116, 0.9f);
  colors[ImGuiCol_ButtonHovered] = color_from_hex(116, 86, 172, 1.0f);
  colors[ImGuiCol_ButtonActive] = color_from_hex(198, 162, 255, 1.0f);
  colors[ImGuiCol_Header] = color_from_hex(82, 55, 145, 0.9f);
  colors[ImGuiCol_HeaderHovered] = color_from_hex(116, 86, 172, 1.0f);
  colors[ImGuiCol_HeaderActive] = color_from_hex(198, 162, 255, 1.0f);
  colors[ImGuiCol_Separator] = color_from_hex(82, 55, 145, 0.35f);
  colors[ImGuiCol_SeparatorHovered] = color_from_hex(116, 86, 172, 0.78f);
  colors[ImGuiCol_SeparatorActive] = color_from_hex(198, 162, 255, 1.0f);
  colors[ImGuiCol_ResizeGrip] = color_from_hex(82, 55, 145, 0.35f);
  colors[ImGuiCol_ResizeGripHovered] = color_from_hex(116, 86, 172, 0.75f);
  colors[ImGuiCol_ResizeGripActive] = color_from_hex(198, 162, 255, 1.0f);
  colors[ImGuiCol_Tab] = color_from_hex(58, 38, 96, 0.95f);
  colors[ImGuiCol_TabHovered] = color_from_hex(116, 86, 172, 1.0f);
  colors[ImGuiCol_TabActive] = color_from_hex(149, 114, 215, 1.0f);
  colors[ImGuiCol_TabUnfocused] = color_from_hex(36, 24, 58, 0.8f);
  colors[ImGuiCol_TabUnfocusedActive] = color_from_hex(104, 75, 177, 1.0f);
#ifdef ImGuiCol_DockingPreview
  colors[ImGuiCol_DockingPreview] = color_from_hex(149, 114, 215, 0.85f);
#endif
#ifdef ImGuiCol_DockingEmptyBg
  colors[ImGuiCol_DockingEmptyBg] = color_from_hex(8, 6, 16, 1.0f);
#endif
  colors[ImGuiCol_PlotLines] = color_from_hex(198, 162, 255, 1.0f);
  colors[ImGuiCol_PlotLinesHovered] = color_from_hex(255, 203, 255, 1.0f);
  colors[ImGuiCol_PlotHistogram] = color_from_hex(149, 114, 215, 1.0f);
  colors[ImGuiCol_PlotHistogramHovered] = color_from_hex(198, 162, 255, 1.0f);
  colors[ImGuiCol_TableHeaderBg] = color_from_hex(36, 24, 58, 1.0f);
  colors[ImGuiCol_TableBorderStrong] = color_from_hex(82, 55, 145, 0.6f);
  colors[ImGuiCol_TableBorderLight] = color_from_hex(82, 55, 145, 0.3f);
  colors[ImGuiCol_TableRowBg] = color_from_hex(20, 16, 30, 0.5f);
  colors[ImGuiCol_TableRowBgAlt] = color_from_hex(26, 20, 40, 0.5f);
  colors[ImGuiCol_TextSelectedBg] = color_from_hex(149, 114, 215, 0.35f);
  colors[ImGuiCol_DragDropTarget] = color_from_hex(255, 203, 255, 1.0f);
  colors[ImGuiCol_NavHighlight] = color_from_hex(198, 162, 255, 1.0f);
  colors[ImGuiCol_NavWindowingHighlight] = color_from_hex(198, 162, 255, 0.7f);
  colors[ImGuiCol_NavWindowingDimBg] = color_from_hex(8, 6, 16, 0.7f);
  colors[ImGuiCol_ModalWindowDimBg] = color_from_hex(8, 6, 16, 0.7f);
}

} // namespace ui
