cmake_minimum_required(VERSION 3.28)

# CXX=clang++ CC=clang cmake -GNinja <sources>

project(matebook-16s-jack-daemon VERSION 1.1 LANGUAGES CXX)

if(NOT (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
    message(FATAL_ERROR "Only the `Clang` compiler is supported")
endif()

if(NOT (CMAKE_GENERATOR STREQUAL "Ninja"))
    message(FATAL_ERROR "Only the `Ninja` generator is supported")
endif()

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_SCAN_FOR_MODULES TRUE)

option(VERBOSE_LOG "Print logs about jack events to stdout/journal")
option(USE_TIDY "Use clang-tidy during compilation")

include(cmake/cxx-module.cmake)

# Enable addition compilation checks
if(USE_TIDY)
    find_program(CMAKE_CXX_CLANG_TIDY NAMES "clang-tidy" REQUIRED)
    message(STATUS "Clang-tidy checks will be running during the build step")
endif()

# Miscellarious files
add_custom_target(misc SOURCES
    .clang-format .clang-tidy
    README.md
    ${PROJECT_NAME}.service
    package/PKGBUILD.in
)

# Wrapper for a POSIX API: isolate C names in C++20 module
add_module(Posix
    Posix/Posix.cxxm
    Posix/File.cxxm
    Posix/HdaDevice.cxxm
    Posix/InputEvent.cxxm
    Posix/Pipe.cxxm
    Posix/Poll.cxxm
    Posix/Pollable.cxxm
    Posix/Signal.cxxm
)

# Wrapper for Alsa library
add_module(Alsa
    Alsa/Alsa.cxxm
)

# Daemon executable
add_executable(${PROJECT_NAME}
    Sources/Main.cxx
    Headers/Globals.hxx
    Headers/OutPipeEnd.hxx
    Sources/FindDeviceNode.cxx
    Headers/JackHandler.hxx
    Sources/JackHandler.cxx
)
target_include_directories(${PROJECT_NAME} PRIVATE Headers)
target_link_libraries(${PROJECT_NAME}
    Posix
    Alsa
    asound
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    __PROGRAM_NAME="${PROJECT_NAME}"
)

if(VERBOSE_LOG)
    target_compile_definitions(${PROJECT_NAME} PRIVATE __VERBOSE_LOG)
endif()

# Additional files
include(cmake/tests.cmake)
include(cmake/archlinux.cmake)
