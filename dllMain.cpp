#pragma comment(lib, "user32")
#include "inicpp.hpp"
#include "proxy.hpp"

#include <windows.h>

#include <string>

bool DEBUG = false;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH: {
    inicpp::IniManager config("font_prop.ini");
    DEBUG = config[""]["debug"];

    Proxy::Init(hModule);

    if (DEBUG)
      std::printf("Injection d2d1.dll successful\n");
  }
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}
