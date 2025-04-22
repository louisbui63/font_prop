import pefile

dll_path = "/home/louisbui63/.wine/drive_c/windows/syswow64/d2d1.dll"
pe = pefile.PE(dll_path)


if hasattr(pe, "DIRECTORY_ENTRY_EXPORT"):
    print("/////////////////////////////proxy.cpp/////////////////////////////////")
    print('#include "proxy.hpp"')
    # for exp in pe.DIRECTORY_ENTRY_EXPORT.symbols:
    #     if exp.name:
    #         print(
    #             f'#pragma comment(linker, "/export:{exp.name.decode()}=hooked__{exp.name.decode()}")'
    #         )

    print(
        """
void Proxy::Init(HMODULE hProxy) {
ProxyModuleHandle = hProxy;

wchar_t realDllPath[MAX_PATH];
GetSystemDirectoryW(realDllPath, MAX_PATH);
wcscat_s(realDllPath, L"\\\\d2d1.dll");
OriginalModuleHandle = LoadLibraryW(realDllPath);
if (OriginalModuleHandle == nullptr) {
    MessageBoxW(nullptr, L"Cannot load original d2d1.dll library", L"Proxy",
            MB_ICONERROR);
    ExitProcess(0);
}

#define RESOLVE(fn) real__##fn = (void*)GetProcAddress(OriginalModuleHandle, #fn)
    """
    )
    for exp in pe.DIRECTORY_ENTRY_EXPORT.symbols:
        if exp.name:
            print(f"RESOLVE({exp.name.decode()});")
    print("""
#undef RESOLVE
}
    """)
    for exp in pe.DIRECTORY_ENTRY_EXPORT.symbols:
        if exp.name:
            print(f"__declspec(naked) void hooked__{exp.name.decode()}() {{")
            print(f"__asm {{ jmp [Proxy::real__{exp.name.decode()}] }}")
            print("}")

    print("/////////////////////////////proxy.hpp/////////////////////////////////")
    print("""
#pragma once
#include <windows.h>

class Proxy {
public:
  static void Init(HMODULE hProxy);

  static inline HMODULE ProxyModuleHandle{};
  static inline HMODULE OriginalModuleHandle{};
  """)
    for exp in pe.DIRECTORY_ENTRY_EXPORT.symbols:
        if exp.name:
            print(f" static inline void *real__{exp.name.decode()}{{}};")
    print("""
};
""")
    print("/////////////////////////////d2d1.def/////////////////////////////////")
    print("""LIBRARY d2d1
EXPORTS""")
    for exp in pe.DIRECTORY_ENTRY_EXPORT.symbols:
        if exp.name:
            print(f"  {exp.name.decode()}=hooked__{exp.name.decode()} @{exp.ordinal}")
