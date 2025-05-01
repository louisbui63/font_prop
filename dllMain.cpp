#pragma comment(lib, "user32")
#pragma comment(lib, "gdi32")
#include "inicpp.hpp"
#include "patcher.hpp"
#include "prop.hpp"
#include "proxy.hpp"

#include <spdlog/spdlog.h>

#include <d2d1.h>
#include <windows.h>
#include <wingdi.h>

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

    patch_function((void *)CreateFontA, (void *)hooked__CreateFontA);
    patch_function((void *)TextOutA, (void *)hooked__TextOutA);
  }
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}
