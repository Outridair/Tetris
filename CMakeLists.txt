cmake_minimum_required(VERSION 3.31)
project(Tetris)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDL2 paths (Homebrew default for Apple Silicon)
include_directories(include)
include_directories(/opt/homebrew/include/SDL2)
link_directories(/opt/homebrew/lib)
file(GLOB SOURCES "src/*.cpp")

add_executable(Tetris src/main.cpp
        src/controls.cpp
        include/pieces.h
        src/pieces.cpp
        src/collision.cpp
        include/collision.h)
target_link_libraries(Tetris SDL2)
