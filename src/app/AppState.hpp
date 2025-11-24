#pragma once

#include "core/Image.hpp"
#include "core/Params.hpp"
#include "util/ErrorManager.hpp"

#include <string>

struct AppState {
  std::string currentPath;
  std::string fileName;

  Image original;
  Image processed;
  Params params;

  bool imageLoaded = false;
  bool needsReprocess = false;

  ErrorManager errors;
};
