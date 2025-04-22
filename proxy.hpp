#pragma once
#include <windows.h>
class Proxy {
public:
  static void Init(HMODULE hProxy);
  static inline HMODULE ProxyModuleHandle{};
  static inline HMODULE OriginalModuleHandle{};
};

static inline void *real__D2D1ConvertColorSpace{};
static inline void *real__D2D1CreateDevice{};
static inline void *real__D2D1CreateDeviceContext{};
static inline void *real__D2D1CreateFactory{};
static inline void *real__D2D1InvertMatrix{};
static inline void *real__D2D1IsMatrixInvertible{};
static inline void *real__D2D1MakeRotateMatrix{};
static inline void *real__D2D1MakeSkewMatrix{};
static inline void *real__D2D1SinCos{};
static inline void *real__D2D1Tan{};
static inline void *real__D2D1Vec3Length{};
