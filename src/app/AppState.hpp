#pragma once

#include "core/Image.hpp"
#include "core/Params.hpp"
#include "gpu/Texture2D.hpp"
#include "util/ErrorManager.hpp"

#include <memory>
#include <string>

struct AppState {
  std::string currentPath;
  std::string fileName;

  Image original;
  Image processed;
  Params params;

  std::unique_ptr<Texture2D> previewTexture;

  bool imageLoaded = false;
  bool needsReprocess = false;

  ErrorManager errors;
};
