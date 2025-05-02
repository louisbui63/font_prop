# BIN=/mnt/part2/owned/msvc/bin/x86 . /mnt/part2/owned/dev/font_prop/msvc-wine/msvcenv-native.sh
# clang++ -std=c++23 -Iinclude -fuse-ld=lld -fms-extensions -fms-compatibility --target=i386-windows-msvc -v -o main.obj -c dllMain.cpp
# clang++ -std=c++23 -Iinclude -fuse-ld=lld -fms-extensions -fms-compatibility --target=i386-windows-msvc -v -o proxy.obj -c proxy.cpp
# clang++ -std=c++23 -Iinclude -fuse-ld=lld -fms-extensions -fms-compatibility --target=i386-windows-msvc -v -o patcher.obj -c patcher.cpp
# clang++ -std=c++23 -Iinclude -fuse-ld=lld -fms-extensions -fms-compatibility --target=i386-windows-msvc -v -o prop.obj -c prop.cpp
# clang++ -std=c++23 -Iinclude -fuse-ld=lld -fms-extensions -fms-compatibility --target=i386-windows-msvc -v -o font.obj -c font.cpp
# clang++ -std=c++23 -fuse-ld=lld -fms-extensions -fms-compatibility --target=i386-windows-msvc -v -o d2d1_p.dll -shared main.obj proxy.obj patcher.obj prop.obj font.obj -Wl,-subsystem:windows,-def:d2d1.def
# clang -fuse-ld=lld --target=x86_64-windows-msvc  -c dllMain.cpp -o d2d1.obj
# clang -fuse-ld=lld --target=x86_64-windows-msvc -v -shared -o d2d1_p.dll d2d1.obj -DEF:d2d1.def -Wl,-subsystem:windows  
#
# PARAMS='-std=c++23 -fms-extensions -L/usr/lib/gcc/i686-w64-mingw32/14-posix -target i686-w64-mingw32 -nostdinc -nostdinc++ -Qunused-arguments -fsjlj-exceptions -isystem /usr/lib/llvm-19/bin/../lib/clang/19/include -isystem /usr/i686-w64-mingw32/include/../../../usr/lib/gcc/i686-w64-mingw32/14-posix/include/c++ -isystem /usr/i686-w64-mingw32/include/../../../usr/lib/gcc/i686-w64-mingw32/14-posix/include/c++/i686-w64-mingw32 -isystem /usr/i686-w64-mingw32/include -Iinclude --target=i686-w64-mingw32'
PARAMS='-std=c++23 -fms-extensions -target i686-w64-mingw32 -I include'
clang++ $PARAMS -v -o main.obj -c dllMain.cpp
clang++ $PARAMS -v -o font.obj -c font.cpp
clang++ $PARAMS -v -o patcher.obj -c patcher.cpp
clang++ $PARAMS -v -o prop.obj -c prop.cpp
clang++ $PARAMS -v -o proxy.obj -c proxy.cpp
clang++ -std=c++23 -fms-extensions -target i686-w64-mingw32 -v -o d2d1_p.dll -static -shared d2d1.def font.obj main.obj patcher.obj prop.obj proxy.obj
