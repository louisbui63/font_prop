#include "proxy.hpp"
void Proxy::Init(HMODULE hProxy) {
  ProxyModuleHandle = hProxy;
  wchar_t realDllPath[MAX_PATH];
  GetSystemDirectoryW(realDllPath, MAX_PATH);
  wcscat_s(realDllPath, L"\\d2d1.dll");
  OriginalModuleHandle = LoadLibraryW(realDllPath);
  if (OriginalModuleHandle == nullptr) {
    MessageBoxW(nullptr, L"Cannot load original d2d1.dll library", L"Proxy",
                MB_ICONERROR);
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

__declspec(naked) void hooked__D2D1ConvertColorSpace() {
  __asm { jmp [Proxy::real__D2D1ConvertColorSpace] }
}
__declspec(naked) void hooked__D2D1CreateDevice() {
  __asm { jmp [Proxy::real__D2D1CreateDevice] }
}
__declspec(naked) void hooked__D2D1CreateDeviceContext() {
  __asm { jmp [Proxy::real__D2D1CreateDeviceContext] }
}
__declspec(naked) void hooked__D2D1CreateFactory() {
  __asm { jmp [Proxy::real__D2D1CreateFactory] }
}
__declspec(naked) void hooked__D2D1InvertMatrix() {
  __asm { jmp [Proxy::real__D2D1InvertMatrix] }
}
__declspec(naked) void hooked__D2D1IsMatrixInvertible() {
  __asm { jmp [Proxy::real__D2D1IsMatrixInvertible] }
}
__declspec(naked) void hooked__D2D1MakeRotateMatrix() {
  __asm { jmp [Proxy::real__D2D1MakeRotateMatrix] }
}
__declspec(naked) void hooked__D2D1MakeSkewMatrix() {
  __asm { jmp [Proxy::real__D2D1MakeSkewMatrix] }
}
__declspec(naked) void hooked__D2D1SinCos() {
  __asm { jmp [Proxy::real__D2D1SinCos] }
}
__declspec(naked) void hooked__D2D1Tan() {
  __asm { jmp [Proxy::real__D2D1Tan] }
}
__declspec(naked) void hooked__D2D1Vec3Length() {
  __asm { jmp [Proxy::real__D2D1Vec3Length] }
}
