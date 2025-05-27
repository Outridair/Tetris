//
// Created by David Rynearson on 26.05.25.
//

#include "../include/Board.hpp"
#include <algorithm>

Board::Board() {

}


bool Board::isOccupied(int x, int y) const {
    if (x < 0 || x>=BOARD_WIDTH || y < 0 || y>=BOARD_HEIGHT)
        return true;
    return grid[y][x] != 0;
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