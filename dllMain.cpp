#include "spdlog/common.h"
#pragma comment(lib, "user32")
#pragma comment(lib, "gdi32")
#include "font.hpp"
#include "patcher.hpp"
#include "prop.hpp"
#include "proxy.hpp"

#include <inicpp.hpp>
#include <spdlog/spdlog.h>

#include <d2d1.h>
#define NOMINMAX
#include <windows.h>
#include <wingdi.h>

#include <filesystem>
#include <string>

spdlog::level::level_enum LOG = spdlog::level::off;

HRESULT __stdcall MyD2D1CreateFactoryHook(
    D2D1_FACTORY_TYPE factoryType, REFIID riid,
    const D2D1_FACTORY_OPTIONS *pFactoryOptions, void **ppIFactory);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH: {
    inicpp::IniManager config("font_prop.ini");

    if (config[""].isKeyExist("log")) {
      std::string l = config[""]["log"];
      if (l == "trace")
        LOG = spdlog::level::trace;
      else if (l == "info")
        LOG = spdlog::level::info;
      else if (l == "warn")
        LOG = spdlog::level::warn;
      else if (l == "error")
        LOG = spdlog::level::err;
      else if (l == "off")
        LOG = spdlog::level::off;
      else
        LOG = spdlog::level::warn;
    } else {
      LOG = spdlog::level::warn;
    }
    spdlog::set_level(LOG);

    Proxy::Init(hModule);

    spdlog::info("Injection d2d1.dll successful\n");

    if (config[""].isKeyExist("font")) {
      set_font(config[""]["font"]);
    }

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
