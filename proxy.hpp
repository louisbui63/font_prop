#pragma once
#include <windows.h>
class Proxy {
public:
  static void Init(HMODULE hProxy);
  static inline HMODULE ProxyModuleHandle{};
  static inline HMODULE OriginalModuleHandle{};
};

extern "C" {
__declspec(dllexport) extern void *real__D2D1ConvertColorSpace;
__declspec(dllexport) extern void *real__D2D1CreateDevice;
__declspec(dllexport) extern void *real__D2D1CreateDeviceContext;
__declspec(dllexport) extern void *real__D2D1CreateFactory;
__declspec(dllexport) extern void *real__D2D1InvertMatrix;
__declspec(dllexport) extern void *real__D2D1IsMatrixInvertible;
__declspec(dllexport) extern void *real__D2D1MakeRotateMatrix;
__declspec(dllexport) extern void *real__D2D1MakeSkewMatrix;
__declspec(dllexport) extern void *real__D2D1SinCos;
__declspec(dllexport) extern void *real__D2D1Tan;
__declspec(dllexport) extern void *real__D2D1Vec3Length;
}
