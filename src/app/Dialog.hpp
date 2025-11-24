#pragma once

#include <string>
#include <variant>

namespace Dialog {
struct FileDialogCancelled {};
struct FileDialogError {
  std::string message;
};
using FileDialogResult = std::variant<std::string,         // success: file path
                                      FileDialogCancelled, // user cancelled
                                      FileDialogError      // error with message
                                      >;

FileDialogResult open_file_dialog();
FileDialogResult save_file_dialog();
} // namespace Dialog
