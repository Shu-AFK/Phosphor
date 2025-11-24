#pragma once

#include "AppState.hpp"

struct AppState;

namespace ui {
void draw_window(AppState &state);

void draw_main_section(AppState &state);
void draw_file_section(AppState &state);
void draw_preview_section(AppState &state);
void draw_parameters_section(AppState &state);
void draw_errors(AppState &state);
} // namespace ui
