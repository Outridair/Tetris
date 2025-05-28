//
// Created by David Rynearson on 26.05.25.
//

#include "../include/Board.hpp"
#include <algorithm>
#include <SDL_log.h>

Board::Board() {
    // set every cell to 0
    for (auto &row : grid) {
        row.fill(0);
    }
}


bool Board::isOccupied(int x, int y) const {
    // 1) Left or right wall
    if (x < 0 || x >= BOARD_WIDTH) {
        SDL_Log("isOccupied: collision with wall (x=%d)", x);
        return true;
    }

    // 2) Above the top of the board (allowed)
    if (y < 0) {
        SDL_Log("isOccupied: above board (y=%d)", y);
        return false;
    }

    // 3) Bottom of the board
    if (y >= BOARD_HEIGHT) {
        SDL_Log("isOccupied: collision with floor (y=%d)", y);
        return true;
    }

    // 4) Inside grid: check if cell is filled
    int cell = grid[y][x];
    SDL_Log("isOccupied: checking grid[%d][%d] = %d", y, x, cell);
    return cell != 0;
}


void Board::occupy(int x, int y) {
    if (x < 0 || x>=BOARD_WIDTH || y < 0 || y>=BOARD_HEIGHT)
        grid[y][x] = 1;
}

int Board::clearFullLines() {
    int linesCleared = 0;
    for (int y = BOARD_HEIGHT -1; y>=0; --y) {
        bool full = true;
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            if (grid[y][x] == 0) {full = false; break; }
        }
        if (full) {
            ++linesCleared;
            for (int row = y; row > 0; --row) {
                grid[row] = grid[row -1];
            }
            grid[0].fill(0);
            ++y;
        }
    }
    return linesCleared;
}