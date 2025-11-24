#include "Dialog.hpp"

#include "nfd.hpp"

Dialog::FileDialogResult Dialog::open_file_dialog() {
  nfdfilteritem_t filters[] = {{"Images", "png,jpg,jpeg"}};
  const nfdfiltersize_t filterCount = sizeof(filters) / sizeof(filters[0]);

  nfdnchar_t *outPath = nullptr;
  nfdresult_t res = NFD::OpenDialog(outPath, filters, filterCount);

  if (res == NFD_OKAY) {
    std::string path(outPath);
    NFD::FreePath(outPath);
    return path;
  }

  if (res == NFD_CANCEL) {
    return FileDialogCancelled{};
  }

  const char *err = NFD::GetError();
  return FileDialogError{err ? err : "Unkown error"};
}

Dialog::FileDialogResult Dialog::save_file_dialog() {
  nfdfilteritem_t filters[] = {{"Images", "png,jpg,jpeg"}};
  const nfdfiltersize_t filterCount = sizeof(filters) / sizeof(filters[0]);

  nfdnchar_t *outPath = nullptr;

  nfdresult_t res = NFD::SaveDialog(outPath, filters, filterCount);

  if (res == NFD_OKAY) {
    std::string path(outPath);
    NFD::FreePath(outPath);

    // Auto append .jpg if no file extension was supplied
    size_t lastSlash = path.find_last_of("/\\");
    std::string filename =
        (lastSlash == std::string::npos) ? path : path.substr(lastSlash + 1);

    if (filename.find('.') == std::string::npos) {
      path += ".jpg";
    }

    return path;
  }

  if (res == NFD_CANCEL) {
    return FileDialogCancelled{};
  }

  const char *err = NFD::GetError();
  return FileDialogError{err ? err : "Unknown error"};
}
