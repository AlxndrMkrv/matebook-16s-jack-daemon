cmake_minimum_required(VERSION 3.28)

# CXX=clang++ CC=clang cmake -GNinja <sources>

project(matebook-16s-jack-daemon CXX)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

#set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")

add_executable(${PROJECT_NAME} Sources/main.cxx
                               Headers/Terminate.hxx
                               Headers/Globals.hxx
                               Headers/EventDev.hxx
                               Sources/EventDev.cxx
                               Headers/Poll.hxx
                               Sources/Poll.cxx
                               Headers/_PollData.hxx
                           )
target_include_directories(${PROJECT_NAME} PRIVATE Headers)

add_custom_target(misc SOURCES README.md .clang-format)

#add_subdirectory(hda-verb)
