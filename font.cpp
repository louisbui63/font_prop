#include "font.hpp"

#include <filesystem>
#include <spdlog/spdlog.h>
#include <windows.h>

std::optional<std::string> FONT = std::nullopt;

std::optional<std::string> get_font_name() {
  return FONT.transform(
      [](auto f) { return std::filesystem::path(f).stem().string(); });
}

void set_font(std::string f) {
  if (AddFontResourceA(f.c_str()) > 0) {
    spdlog::info("Font {} successfully loaded", f);
    FONT = f;
  }

  else
    spdlog::error("Couldn't load font {}", f);
}
