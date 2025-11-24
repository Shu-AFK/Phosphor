#pragma once

#include <string>
#include <string_view>

namespace util {

inline std::string filename_from_path(std::string_view path) {
  if (path.empty())
    return {};

  size_t pos = path.find_last_of("/\\");
  if (pos == std::string_view::npos) {
    return std::string(path);
  }

  if (pos + 1 >= path.size()) {
    return {};
  }

  return std::string(path.substr(pos + 1));
}

} // namespace util
