//
// Created by David Rynearson on 27.05.25.
//

#include "../include/Renderer.hpp"
#include <SDL.h>

void Renderer::drawBoard(const Board& board) {
    for(int y=0;y<BOARD_HEIGHT;++y) {
        for(int x=0;x<BOARD_WIDTH;++x) {
            Tetromino::Type t = board.getCellType(x,y);
            // convert int back to enum, get its color
            SDL_Color c = Tetromino(t).getColor();
            drawCell(x, y, c);
        }
    }
}

void Renderer::drawTetromino(const Tetromino &t) {
    const auto& shape = t.currentShape();
    SDL_Color color = t.getColor();
    for (int i =0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (shape[i][j]) {
                drawCell(t.x + j, t.y + i, color);
            }
        }
    }
}

void Renderer::drawCell(int x, int y, SDL_Color color) {
    SDL_Rect rect{
        offsetX + x * CELL_SIZE,
        offsetY + y * CELL_SIZE,
        CELL_SIZE,
        CELL_SIZE
    };
    SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(ren, &rect);

    //Draw Border
    SDL_SetRenderDrawColor(ren, 50, 50, 50, 255);
    SDL_RenderDrawRect(ren, &rect);
}
