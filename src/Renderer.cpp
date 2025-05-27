//
// Created by David Rynearson on 27.05.25.
//

#include "../include/Renderer.hpp"
#include <SDL.h>

void Renderer::drawBoard(const Board &board) {
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            if (board.isOccupied(x, y)) {
                drawCell(x, y, SDL_Color{255, 255, 255, 255});
            }
        }
    }
}

void Renderer::drawTetromino(const Tetromino &t) {
    const auto& shape = t.currentShape();
    for (int i =0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (shape[i][j]) {
                drawCell(t.x + j, t.y + i, SDL_Color{200,200,200,255});
            }
        }
    }
}

void Renderer::drawCell(int x, int y, SDL_Color color) {
    const int cellSize = 30;
    SDL_Rect rect{x * cellSize, y * cellSize, cellSize, cellSize};
    SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(ren, &rect);

    //Draw Border
    SDL_SetRenderDrawColor(ren, 50, 50, 50, 255);
    SDL_RenderDrawRect(ren, &rect);
}
