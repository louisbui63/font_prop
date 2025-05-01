BIN=/mnt/part2/owned/msvc/bin/x86 . /mnt/part2/owned/dev/font_prop/msvc-wine/msvcenv-native.sh
clang++ -std=c++23 -Iinclude -fuse-ld=lld -fms-extensions -fms-compatibility --target=i386-windows-msvc -v -o main.obj -c dllMain.cpp
clang++ -std=c++23 -Iinclude -fuse-ld=lld -fms-extensions -fms-compatibility --target=i386-windows-msvc -v -o proxy.obj -c proxy.cpp
clang++ -std=c++23 -Iinclude -fuse-ld=lld -fms-extensions -fms-compatibility --target=i386-windows-msvc -v -o patcher.obj -c patcher.cpp
clang++ -std=c++23 -fuse-ld=lld -fms-extensions -fms-compatibility --target=i386-windows-msvc -v -o d2d1_p.dll -shared main.obj proxy.obj patcher.obj -Wl,-subsystem:windows,-def:d2d1.def
# clang -fuse-ld=lld --target=x86_64-windows-msvc  -c dllMain.cpp -o d2d1.obj
# clang -fuse-ld=lld --target=x86_64-windows-msvc -v -shared -o d2d1_p.dll d2d1.obj -DEF:d2d1.def -Wl,-subsystem:windows  
