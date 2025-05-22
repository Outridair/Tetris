//
// Created by David Rynearson on 18.05.25.
//
#include "../include/pieces.h"

int I_piece[TETROMINO_SIZE][TETROMINO_SIZE] = {
    {0,0,0,0},
    {1,1,1,1},
    {0,0,0,0},
    {0,0,0,0}
};

int O_piece[TETROMINO_SIZE][TETROMINO_SIZE] = {
    {0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0}
};

int T_piece[TETROMINO_SIZE][TETROMINO_SIZE] = {
    {0,0,0,0},
    {1,1,1,0},
    {0,1,0,0},
    {0,0,0,0}
};

int S_piece[TETROMINO_SIZE][TETROMINO_SIZE] = {
    {0,0,0,0},
    {0,1,1,0},
    {1,1,0,0},
    {0,0,0,0}
};

int Z_piece[TETROMINO_SIZE][TETROMINO_SIZE] = {
    {0,0,0,0},
    {1,1,0,0},
    {0,1,1,0},
    {0,0,0,0}
};

int J_piece[TETROMINO_SIZE][TETROMINO_SIZE] = {
    {0,0,0,0},
    {1,1,1,0},
    {0,0,1,0},
    {0,0,0,0}
};

int L_piece[TETROMINO_SIZE][TETROMINO_SIZE] = {
    {0, 0, 0, 0},
    {1, 1, 1, 0},
    {1, 0, 0, 0},
    {0, 0, 0, 0}
};

int (*getPieceByType(TetrominoType type))[TETROMINO_SIZE] {
    switch (type) {
        case I: return I_piece;
        case O: return O_piece;
        case T: return T_piece;
        case S: return S_piece;
        case Z: return Z_piece;
        case J: return J_piece;
        case L: return L_piece;
        default: return O_piece;
    }
}