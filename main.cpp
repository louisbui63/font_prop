#include <d2d1.h>

// Original function type
typedef HRESULT(STDMETHODCALLTYPE *OriginalDrawText)(
    ID2D1RenderTarget *pThis, __in_ecount(stringLength) PCWSTR string,
    UINT32 stringLength, IDWriteTextFormat *textFormat,
    __in const D2D1_RECT_F *layoutRect, ID2D1Brush *defaultFillBrush,
    D2D1_DRAW_TEXT_OPTIONS options, DWRITE_MEASURING_MODE measuringMode);

// Our detour function
HRESULT STDMETHODCALLTYPE MyDrawText(ID2D1RenderTarget *pThis, PCWSTR string,
                                     UINT32 stringLength,
                                     IDWriteTextFormat *textFormat,
                                     const D2D1_RECT_F *layoutRect,
                                     ID2D1Brush *defaultFillBrush,
                                     D2D1_DRAW_TEXT_OPTIONS options,
                                     DWRITE_MEASURING_MODE measuringMode) {
  // Your interception code here
  // Modify parameters if needed

  // Call original function
  OriginalDrawText original =
      (OriginalDrawText)((void **)pThis->lpVtbl)[8]; // DrawText is at index 8
  return original(pThis, string, stringLength, textFormat, layoutRect,
                  defaultFillBrush, options, measuringMode);
}

// Installation function
void HookDrawText(ID2D1RenderTarget *pTarget) {
  // Get the vtable pointer
  void **vtable = *(void ***)pTarget;

  // Save original
  static OriginalDrawText original = (OriginalDrawText)vtable[8];

  // Make memory writable
  DWORD oldProtect;
  VirtualProtect(&vtable[8], sizeof(void *), PAGE_READWRITE, &oldProtect);

  // Replace with our detour
  vtable[8] = &MyDrawText;

  // Restore protection
  VirtualProtect(&vtable[8], sizeof(void *), oldProtect, &oldProtect);
}
