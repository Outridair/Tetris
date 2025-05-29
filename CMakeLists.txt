cmake_minimum_required(VERSION 3.31)
project(TetrisSDL)

set(CMAKE_CXX_STANDARD 17)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)

add_executable(TetrisSDL
        src/main.cpp
        src/Game.cpp  include/Game.hpp
        src/Board.cpp include/Board.hpp
        src/Tetromino.cpp include/Tetromino.hpp
        src/Renderer.cpp include/Renderer.hpp
)
target_link_libraries(TetrisSDL PRIVATE
        SDL2::SDL2            # brings in SDL2 headers & libs
        SDL2_ttf::SDL2_ttf    # brings in TTF headers & libs
)