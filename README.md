# matebook-16s-jack-daemon

This is my user-space approach to solve the well-known bug with the SN6140 SoC 
pins connection on the HUAWEI MateBook 14s/16s series.

I hope to solve the problem in 
[kernel-space](https://github.com/AlxndrMkrv/matebook-16s-sound-fix) 
but until that I'm relying on this tiny service.

## Compilation

Since the project is based on C++20 modules it can be compilled only with
Clang/Ninja. Also, I've encontered some problems with linking the Release build 
type, so the Debug one should be prefered for now.

``` bash
$ cmake <path_to_project> \
        --toolchain <path_to_project>/cmake/clang-ninja.cmake \
        -GNinja
$ cmake --build .
```

## Build an Arch Linux package

``` bash
$ cmake --build . --target archlinux_package
```

## Problems

If you use Bluetooth audio output devices like headphones, disconnecting them 
leads to a restoration of the SoC configuration that the daemon cannot detect. 
To resolve this, you have to restart the service with the following command:

``` bash
systemctl restart matebook-16s-jack-daemon
```

Also, restarting the service is the first option if the sound is not outputting 
properly...

## TODO

Get rid of the `libasound` dependency by reading the current jack state from 
`/proc/asound/card0/codec#0`

## Information sources

https://wiki.archlinux.org/title/Huawei_MateBook_14s#Audio
