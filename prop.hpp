#pragma once

#define NOMINMAX
#include <windows.h>

HFONT __stdcall hooked__CreateFontA(
    [in] int cHeight, [in] int cWidth, [in] int cEscapement,
    [in] int cOrientation, [in] int cWeight, [in] DWORD bItalic,
    [in] DWORD bUnderline, [in] DWORD bStrikeOut, [in] DWORD iCharSet,
    [in] DWORD iOutPrecision, [in] DWORD iClipPrecision, [in] DWORD iQuality,
    [in] DWORD iPitchAndFamily, [in] LPCSTR pszFaceName);

BOOL __stdcall hooked__TextOutA([in] HDC hdc, [in] int x, [in] int y,
                                [in] LPCSTR lpString, [in] int c);
