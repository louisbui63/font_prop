#pragma comment(lib, "user32")
#include "inicpp.hpp"
#include "patcher.hpp"
#include "proxy.hpp"

#include <spdlog/spdlog.h>

#include <d2d1.h>
#include <windows.h>

#include <string>

bool DEBUG = false;

HRESULT __stdcall MyD2D1CreateFactoryHook(
    D2D1_FACTORY_TYPE factoryType, REFIID riid,
    const D2D1_FACTORY_OPTIONS *pFactoryOptions, void **ppIFactory);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH: {
    inicpp::IniManager config("font_prop.ini");
    DEBUG = config[""]["debug"];

    Proxy::Init(hModule);

    if (DEBUG)
      spdlog::set_level(spdlog::level::debug);
    else
      spdlog::set_level(spdlog::level::warn);
    spdlog::info("Injection d2d1.dll successful\n");

    patch_function(real__D2D1CreateFactory, (void *)MyD2D1CreateFactoryHook);
  }
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}

void **get_vtable(void *pInterface) {
  return *reinterpret_cast<void ***>(pInterface);
}

typedef HRESULT(__stdcall *OriginalDrawText)(
    ID2D1RenderTarget *pThis, __in_ecount(stringLength) PCWSTR string,
    UINT32 stringLength, IDWriteTextFormat *textFormat,
    __in const D2D1_RECT_F *layoutRect, ID2D1Brush *defaultFillBrush,
    D2D1_DRAW_TEXT_OPTIONS options, DWRITE_MEASURING_MODE measuringMode);

static OriginalDrawText original_draw_text;

HRESULT __stdcall hooked_draw_text(ID2D1RenderTarget *pThis, PCWSTR string,
                                   UINT32 stringLength,
                                   IDWriteTextFormat *textFormat,
                                   const D2D1_RECT_F *layoutRect,
                                   ID2D1Brush *defaultFillBrush,
                                   D2D1_DRAW_TEXT_OPTIONS options,
                                   DWRITE_MEASURING_MODE measuringMode) {

  return original_draw_text(pThis, string, stringLength, textFormat, layoutRect,
                            defaultFillBrush, options, measuringMode);
}

void HookDrawText(ID2D1RenderTarget *pTarget) {
  void **vtable = get_vtable(pTarget);

  const int DRAW_TEXT_INDEX = 8;

  original_draw_text =
      reinterpret_cast<OriginalDrawText>(vtable[DRAW_TEXT_INDEX]);

  DWORD old_protect;
  VirtualProtect(&vtable[DRAW_TEXT_INDEX], sizeof(void *), PAGE_READWRITE,
                 &old_protect);

  vtable[DRAW_TEXT_INDEX] = (void *)&hooked_draw_text;

  VirtualProtect(&vtable[DRAW_TEXT_INDEX], sizeof(void *), old_protect,
                 &old_protect);

  FlushInstructionCache(GetCurrentProcess(), &vtable[DRAW_TEXT_INDEX],
                        sizeof(void *));
}

typedef HRESULT __stdcall (*type__D2D1CreateFactory)(
    D2D1_FACTORY_TYPE factoryType, REFIID riid,
    const D2D1_FACTORY_OPTIONS *pFactoryOptions, void **ppIFactory);

HRESULT __stdcall MyD2D1CreateFactoryHook(
    D2D1_FACTORY_TYPE factoryType, REFIID riid,
    const D2D1_FACTORY_OPTIONS *pFactoryOptions, void **ppIFactory) {

  spdlog::info("Intercepted D2D1CreateFactory");

  auto out = ((type__D2D1CreateFactory)GET_TRAMPOLINE(MyD2D1CreateFactoryHook))(
      factoryType, riid, pFactoryOptions, ppIFactory);

  return out;
}
