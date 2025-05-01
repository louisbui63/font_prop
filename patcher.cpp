#include "patcher.hpp"
#include "spdlog/spdlog.h"
#include "windows.h"
#include <cstdint>
#include <map>

std::map<size_t, size_t> TRAMPOLINE_MAP{};

// this would at least work for 32bit but beware on 64bit as we use a 32bit rel
// jmp
void patch_function(void *patched_function, void *hook_function) {
  SetLastError(1);
  DWORD old_protect;
  if (!VirtualProtect(patched_function, 5, PAGE_EXECUTE_READWRITE,
                      &old_protect)) {
    spdlog::error("Error:{}", GetLastError());
  }

  uint8_t original_bytes[5];
  memcpy(original_bytes, patched_function, 5);

  void *trampoline = VirtualAlloc(nullptr, 20, MEM_COMMIT | MEM_RESERVE,
                                  PAGE_EXECUTE_READWRITE);
  memcpy(trampoline, original_bytes, 5);

  *((uint8_t *)trampoline + 5) = 0x90;
  *((uint8_t *)trampoline + 6) = 0xE9;
  *((size_t *)((uint8_t *)trampoline + 7)) =
      (size_t)patched_function + 5 - (size_t)trampoline - 6 - 5;

  TRAMPOLINE_MAP[(size_t)hook_function] = (size_t)trampoline;

  uint8_t jmp[5] = {0xE9, 0, 0, 0, 0};
  *(uint32_t *)(jmp + 1) =
      (uint32_t)((size_t)hook_function - ((size_t)(patched_function) + 5));

  memcpy(patched_function, jmp, 5);

  VirtualProtect(patched_function, 5, old_protect, &old_protect);
  FlushInstructionCache(GetCurrentProcess(), patched_function, 5);
}

size_t __get_trampoline(size_t x) { return TRAMPOLINE_MAP[x]; }
