#include "prop.hpp"

#include "patcher.hpp"
#include <spdlog/spdlog.h>

HFONT __stdcall hooked__CreateFontA(
    [in] int cHeight, [in] int cWidth, [in] int cEscapement,
    [in] int cOrientation, [in] int cWeight, [in] DWORD bItalic,
    [in] DWORD bUnderline, [in] DWORD bStrikeOut, [in] DWORD iCharSet,
    [in] DWORD iOutPrecision, [in] DWORD iClipPrecision, [in] DWORD iQuality,
    [in] DWORD iPitchAndFamily, [in] LPCSTR pszFaceName) {
  spdlog::info("Intercepted {}", __PRETTY_FUNCTION__);

  auto out =
      ((decltype(hooked__CreateFontA) *)GET_TRAMPOLINE(hooked__CreateFontA))(
          cHeight, cWidth, cEscapement, cOrientation, cWeight, bItalic,
          bUnderline, bStrikeOut, iCharSet, iOutPrecision, iClipPrecision,
          iQuality, iPitchAndFamily, pszFaceName);

  return out;
}

BOOL __stdcall hooked__TextOutA([in] HDC hdc, [in] int x, [in] int y,
                                [in] LPCSTR lpString, [in] int c) {
  spdlog::info("Intercepted {}", __PRETTY_FUNCTION__);

  auto out = ((decltype(hooked__TextOutA) *)GET_TRAMPOLINE(hooked__TextOutA))(
      hdc, x, y, lpString, c);

  return out;
}
