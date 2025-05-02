#include "proxy.hpp"
#include "d2d1.h"
#include <cstdio>

#include "spdlog/spdlog.h"

__declspec(dllexport) void *real__D2D1ConvertColorSpace{};
__declspec(dllexport) void *real__D2D1CreateDevice{};
__declspec(dllexport) void *real__D2D1CreateDeviceContext{};
__declspec(dllexport) void *real__D2D1CreateFactory{};
__declspec(dllexport) void *real__D2D1InvertMatrix{};
__declspec(dllexport) void *real__D2D1IsMatrixInvertible{};
__declspec(dllexport) void *real__D2D1MakeRotateMatrix{};
__declspec(dllexport) void *real__D2D1MakeSkewMatrix{};
__declspec(dllexport) void *real__D2D1SinCos{};
__declspec(dllexport) void *real__D2D1Tan{};
__declspec(dllexport) void *real__D2D1Vec3Length{};

void Proxy::Init(HMODULE hProxy) {
  ProxyModuleHandle = hProxy;
  wchar_t realDllPath[MAX_PATH];
  GetSystemDirectoryW(realDllPath, MAX_PATH);
  wcscat_s(realDllPath, L"\\d2d1.dll");
  OriginalModuleHandle = LoadLibraryW(realDllPath);
  if (OriginalModuleHandle == nullptr) {
    spdlog::error("Cannot load original d2d1.dll library");
    ExitProcess(0);
  }
#define RESOLVE(fn)                                                            \
  real__##fn = (void *)GetProcAddress(OriginalModuleHandle, #fn)

  RESOLVE(D2D1ConvertColorSpace);
  RESOLVE(D2D1CreateDevice);
  RESOLVE(D2D1CreateDeviceContext);
  RESOLVE(D2D1CreateFactory);
  RESOLVE(D2D1InvertMatrix);
  RESOLVE(D2D1IsMatrixInvertible);
  RESOLVE(D2D1MakeRotateMatrix);
  RESOLVE(D2D1MakeSkewMatrix);
  RESOLVE(D2D1SinCos);
  RESOLVE(D2D1Tan);
  RESOLVE(D2D1Vec3Length);
#undef RESOLVE
}

extern "C" {
__attribute__((naked)) void hooked__D2D1ConvertColorSpace() {
  __asm__("jmp *_real__D2D1ConvertColorSpace;" :);
}
__attribute__((naked)) void hooked__D2D1CreateDevice() {
  __asm__("jmp *_real__D2D1CreateDevice;" :);
}
__attribute__((naked)) void hooked__D2D1CreateDeviceContext() {
  __asm__("jmp *_real__D2D1CreateDeviceContext;" :);
}
__attribute__((naked)) void hooked__D2D1CreateFactory() {
  __asm__("jmp *_real__D2D1CreateFactory;" :);
}
__attribute__((naked)) void hooked__D2D1InvertMatrix() {
  __asm__("jmp *_real__D2D1InvertMatrix;" :);
}
__attribute__((naked)) void hooked__D2D1IsMatrixInvertible() {
  __asm__("jmp *_real__D2D1IsMatrixInvertible;" :);
}
__attribute__((naked)) void hooked__D2D1MakeRotateMatrix() {
  __asm__("jmp *_real__D2D1MakeRotateMatrix;" :);
}
__attribute__((naked)) void hooked__D2D1MakeSkewMatrix() {
  __asm__("jmp *_real__D2D1MakeSkewMatrix;" :);
}
__attribute__((naked)) void hooked__D2D1SinCos() {
  __asm__("jmp *_real__D2D1SinCos;" :);
}
__attribute__((naked)) void hooked__D2D1Tan() {
  __asm__("jmp *_real__D2D1Tan;" :);
}
__attribute__((naked)) void hooked__D2D1Vec3Length() {
  __asm__("jmp *_real__D2D1Vec3Length;" :);
}
}
