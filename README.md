# matebook-16s-jack-daemon

This is my user-space approach to solve the well-known bug with the SN6140 SoC 
pins connection on the HUAWEI MateBook 14s/16s series.

I hope to solve this in kernel-space but until that I will rely on this tiny 
service.

## Compilation

Since the project is based on C++20 modules it can be compilled only with
Clang/Ninja

``` bash
$ cmake <path_to>/matebook-16s-jack-daemon \
        --toolchain <path_to>/matebook-16s-jack-daemon/cmake/clang-ninja.cmake \
        -GNinja
$ cmake --build .
```

## Information sources

`/proc/asound/card0/codec#0`
