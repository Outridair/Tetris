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

static constexpr Color pieceColors[7] = {
    /* I */ {  0, 255, 255 },
    /* O */ { 255, 255,   0 },
    /* T */ { 128,   0, 128 },
    /* S */ {   0, 255,   0 },
    /* Z */ { 255,   0,   0 },
    /* J */ {   0,   0, 255 },
    /* L */ { 255, 165,   0 }
};

Color getPieceColor(TetrominoType type) {
    return pieceColors[static_cast<int>(type)];
}