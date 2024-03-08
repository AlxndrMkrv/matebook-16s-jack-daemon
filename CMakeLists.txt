cmake_minimum_required(VERSION 3.28)

# CXX=clang++ CC=clang cmake -GNinja <sources>

project(matebook-16s-jack-daemon CXX)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_SCAN_FOR_MODULES TRUE)

#set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")

add_library(c_wrappers)
target_sources(c_wrappers PUBLIC FILE_SET CXX_MODULES FILES
               Modules/Wrappers.cxxm)
target_include_directories(c_wrappers PRIVATE Headers)

add_executable(${PROJECT_NAME} Sources/main.cxx
                               Headers/Terminate.hxx
                               Headers/Globals.hxx)
target_include_directories(${PROJECT_NAME} PRIVATE Headers)
target_link_libraries(${PROJECT_NAME} c_wrappers)

add_custom_target(misc SOURCES README.md .clang-format)

#add_subdirectory(hda-verb)
