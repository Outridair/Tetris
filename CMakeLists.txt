cmake_minimum_required(VERSION 3.31)
project(TetrisSDL)

set(CMAKE_CXX_STANDARD 17)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(TetrisSDL
        src/main.cpp
        src/Game.cpp  include/Game.hpp
        src/Board.cpp include/Board.hpp
        src/Tetromino.cpp include/Tetromino.hpp
        src/Renderer.cpp include/Renderer.hpp
)
target_link_libraries(TetrisSDL ${SDL2_LIBRARIES})
