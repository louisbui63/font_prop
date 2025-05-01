#pragma once

#define GET_TRAMPOLINE(x) __get_trampoline((size_t)(x))

size_t __get_trampoline(size_t x);
void patch_function(void *patched_function, void *hook_function);
