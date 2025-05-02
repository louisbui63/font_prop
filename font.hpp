#pragma once

#include <optional>
#include <string>

extern std::optional<std::string> FONT;

std::optional<std::string> get_font_name();
void set_font(std::string f);
