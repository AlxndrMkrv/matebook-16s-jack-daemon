cmake_minimum_required(VERSION 3.28)

# CXX=clang++ CC=clang cmake -GNinja <sources>

project(matebook-16s-jack-daemon CXX)
enable_testing()

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_SCAN_FOR_MODULES TRUE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

# Enable addition compilation checks

option(USE_TIDY "Use clang-tidy during compilation")

if(USE_TIDY)
    find_program(CLANG_TIDY_EXEC NAMES "clang-tidy" REQUIRED)
    message(STATUS "Clang-tidy checks will be running during the build step")
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXEC})
else()
    message(STATUS "Clang-tidy checks will not be included")
endif()

# Custom targets

add_custom_target(toolchains SOURCES cmake/clang-ninja.cmake)

# Wrapper for a POSIX API: isolate C names in C++20 module

add_library(posix)
target_sources(posix PUBLIC FILE_SET CXX_MODULES FILES
               Posix/File.cxxm
               Posix/HdaDevice.cxxm
               Posix/InputEvent.cxxm
               Posix/Pipe.cxxm
               Posix/Poll.cxxm
               Posix/Pollable.cxxm
               Posix/Posix.cxxm
               Posix/Signal.cxxm
)
target_include_directories(posix PRIVATE Headers)

# Wrapper for Alsa library

add_library(alsa)
target_sources(alsa PUBLIC FILE_SET CXX_MODULES FILES
               Alsa/Alsa.cxxm
)
target_include_directories(alsa PRIVATE Headers)

# Daemon executable

add_executable(${PROJECT_NAME} Sources/Main.cxx
                               Headers/Globals.hxx
                               Headers/OutPipeEnd.hxx
                               Sources/FindDeviceNode.cxx
                               Headers/JackHandler.hxx
                               Sources/JackHandler.cxx
)
target_include_directories(${PROJECT_NAME} PRIVATE Headers)
target_link_libraries(${PROJECT_NAME} posix alsa asound)

add_custom_target(misc SOURCES .clang-format .clang-tidy
                               README.md
                               ${PROJECT_NAME}.service
                               #package/PKGBUILD
)

# Tests

add_custom_target(tests)

add_executable(pipe_test EXCLUDE_FROM_ALL Tests/Pipe.cxx)
add_dependencies(tests pipe_test)
target_link_libraries(pipe_test posix)
add_test(NAME pipe COMMAND pipe_test)

add_executable(poll_test EXCLUDE_FROM_ALL Tests/Poll.cxx)
add_dependencies(tests poll_test)
target_link_libraries(poll_test posix)
add_test(NAME poll COMMAND poll_test)
