#pragma once

#include <deque>
#include <string>

class ErrorManager {
public:
  void push(std::string msg) {
    errors.push_back(std::move(msg));
    requestPopup = true;
  }

  bool should_open_popup() {
    if (requestPopup && !errors.empty()) {
      requestPopup = false;
      return true;
    }
    return false;
  }

  bool empty() const { return errors.empty(); }

  const std::string &current() const { return errors.front(); }

  void pop() {
    errors.pop_front();
    if (!errors.empty())
      requestPopup = true;
  }

private:
  std::deque<std::string> errors;
  bool requestPopup = false;
};
