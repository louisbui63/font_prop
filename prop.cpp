#include "prop.hpp"
#include "font.hpp"
#include "patcher.hpp"

#include <cstdint>
#include <cstdlib>
#include <spdlog/spdlog.h>
#include <tinyutf8.h>

#define SHADOW_LENIENCY 10

void proportionalize(HDC hdc, int *x, int *y, char c) {
  static int prevx = 0;
  static int prevy = -100;
  static int prev_hookedx = 0;
  static int nextx = 0;

  // printf("%c", c);

  ABCFLOAT abc;
  GetCharABCWidthsFloatA(hdc, (wchar_t)c, (wchar_t)c, &abc);
  int w = (int)(abc.abcfA + abc.abcfB + abc.abcfC);
  // printf("%c(%i,%i:%i)", c, *x, *y, w);

  if (std::abs(*x - prevx) < SHADOW_LENIENCY &&
      std::abs(*y - prevy) < SHADOW_LENIENCY) {
    // Hack to deal with shadow effects
    prevx = *x;
    *x += prev_hookedx - prevx;
    prev_hookedx = *x;
    return;
  }

  if (*x < prevx || std::abs(*y - prevy) > 5) {
    spdlog::trace("Line Break");
    prevx = nextx = prev_hookedx = *x;
    prevy = *y;
  } else if (prevx == *x) {
    *x = prev_hookedx;
    return;
  }

  prevx = *x;
  prev_hookedx = nextx;
  *x = nextx;

  nextx += w;
}

tiny_utf8::string fullwidth_to_prop(tiny_utf8::string in) {
  tiny_utf8::string out;
  for (char32_t c : in) {
    if (c == 0x3000) {
      out.push_back(' ');
    } else if (c > 0xff00 && c < 0xff5f) {
      out.push_back(c - 0xff00 + 0x0020);
    } else if (c == U'”') {
      out.push_back('"');
    } else if (c == U'’') {
      out.push_back('\'');
    } else if (c == U'【') {
      out.push_back('[');
    } else if (c == U'】') {
      out.push_back(']');
    } else if (c == U'ー') {
      out.push_back('-');
    }

    else if (c == ' ' || c == '\0') {
      out.push_back(c);
    } else {
      spdlog::warn("{}({}) was directly forwarded",
                   tiny_utf8::string(c).cpp_str(), (uint32_t)c);
      out.push_back(c);
    }
  }
  return out;
}

std::string shift_jis_to_utf8(LPCSTR in, int length) {
  // convert multibyte to  wide char
  int utf16size = MultiByteToWideChar(932, 0, in, length, 0, 0);
  LPWSTR pUTF16 = new WCHAR[utf16size];
  MultiByteToWideChar(932, 0, in, length, pUTF16, utf16size);

  // convert wide char to multi byte utf-8 before writing to a file
  std::string out;
  out.resize(WideCharToMultiByte(CP_UTF8, 0, pUTF16, -1, NULL, 0, 0, 0));
  char *ptr = &out[0];
  WideCharToMultiByte(CP_UTF8, 0, pUTF16, -1, (char *)ptr, out.size(), 0, 0);

  delete[] pUTF16;
  return out;
}
std::string shift_jis_to_utf8(LPCSTR in) { return shift_jis_to_utf8(in, -1); }

HFONT __stdcall hooked__CreateFontA(
    [in] int cHeight, [in] int cWidth, [in] int cEscapement,
    [in] int cOrientation, [in] int cWeight, [in] DWORD bItalic,
    [in] DWORD bUnderline, [in] DWORD bStrikeOut, [in] DWORD iCharSet,
    [in] DWORD iOutPrecision, [in] DWORD iClipPrecision, [in] DWORD iQuality,
    [in] DWORD iPitchAndFamily, [in] LPCSTR pszFaceName) {
  spdlog::trace("Intercepted {}", __PRETTY_FUNCTION__);
  spdlog::trace("({},{},{},{},{},{},{},{},{},{},{},{},{},{})", cHeight, cWidth,
                cEscapement, cOrientation, cWeight, bItalic, bUnderline,
                bStrikeOut, iCharSet, iOutPrecision, iClipPrecision, iQuality,
                iPitchAndFamily, shift_jis_to_utf8(pszFaceName));

  auto f = get_font_name();
  auto out =
      ((decltype(hooked__CreateFontA) *)GET_TRAMPOLINE(hooked__CreateFontA))(
          cHeight, 0, cEscapement, cOrientation, 500, bItalic, bUnderline,
          bStrikeOut, ANSI_CHARSET, iOutPrecision, iClipPrecision, iQuality,
          iPitchAndFamily, f ? f->c_str() : pszFaceName);

  return out;
}

int CALLBACK EnumFontFamExProc(const LOGFONT *lpelfe, const TEXTMETRIC *lpntme,
                               DWORD FontType, LPARAM lParam) {
  spdlog::error("{}", lpelfe->lfFaceName);

  return 1;
}

void enum_fonts(HDC hdc) {
  LOGFONTA l;
  l.lfCharSet = 0;
  EnumFontFamiliesExA(hdc, &l, EnumFontFamExProc, 0, 0);
}

int lastlength = 0;

BOOL __stdcall hooked__TextOutA([in] HDC hdc, [in] int x, [in] int y,
                                [in] LPCSTR lpString, [in] int c) {

  spdlog::trace("Intercepted {}", __PRETTY_FUNCTION__);
  spdlog::trace("({},{},{},{},{})", (uintptr_t)hdc, x, y,
                fullwidth_to_prop(shift_jis_to_utf8(lpString)).cpp_str(), c);

  auto str = fullwidth_to_prop(shift_jis_to_utf8(lpString));
  if (str.size() > lastlength) {
    spdlog::trace("({}:{}[{}]){}->{}", x, y, c, shift_jis_to_utf8(lpString),
                  str.cpp_str());

    spdlog::trace("{:x}", GetTextColor(hdc));
  }
  lastlength = str.size();

  proportionalize(hdc, &x, &y, str[0]);

  // uint16_t codepoint = (lpString[0] << 8) + lpString[1];
  // spdlog::warn("c:{}", codepoint);

  auto out = ((decltype(hooked__TextOutA) *)GET_TRAMPOLINE(hooked__TextOutA))(
      hdc, x, y, fullwidth_to_prop(shift_jis_to_utf8(lpString)).c_str(),
      c == 2 ? 1 : c);

  return out;
}
