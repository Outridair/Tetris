//
// Created by David Rynearson on 22.05.25.
//

#include "../include/collision.h"

bool canMove(int newX, int newY, int piece[4][4], int board[20][10]) {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (piece[row][col]) {
                int x = newX + col;
                int y = newY + row;

                // Check bounds
                if (x < 0 || x >= 10 || y >= 20) {
                    return false;
                }

                // Check collision with board
                if (y >= 0 && board[y][x]) {
                    return false;
                }
            }
        }
    }
    return true;
}
