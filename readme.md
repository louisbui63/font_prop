# Font Prop 
Font proportionalizer for some System-NNN games that can sometimes be used to replace that of the official English release, especially if it doesn't work on Wine.

## Compilation
This project will happily compile on Linux with Clang + MSVC (installed from, e.g. [msvc-wine](https://github.com/mstorsjo/msvc-wine/)) or Clang + Mingw (from e.g. [llvm-mingw](https://github.com/mstorsjo/llvm-mingw/issues)).
Cross files are included for reference. \
compile with 
```sh
meson setup build --cross-file your-cross-file.ini
meson compile -C build
```
Compilation might be possible with `mingw32-gcc` or the stock MSVC toolchain, but this hasn't been attempted.

## Usage
 - Copy the generated `d2d1.dll` next to the game's `.exe`
 - Copy `font_prop.ini` next to the game's `exe`
 - The font can be changed in `font_prop.ini`. The default font of the game will be used if you don't specify one
