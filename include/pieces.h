//
// Created by David Rynearson on 18.05.25.
//

#ifndef PIECES_H
#define PIECES_H
#include <SDL_stdinc.h>
const int TETROMINO_SIZE = 4;
enum TetrominoType {
    I, O, T, S, Z, J, L
};

extern int I_piece[TETROMINO_SIZE][TETROMINO_SIZE];
extern int O_piece[TETROMINO_SIZE][TETROMINO_SIZE];
extern int T_piece[TETROMINO_SIZE][TETROMINO_SIZE];
extern int S_piece[TETROMINO_SIZE][TETROMINO_SIZE];
extern int Z_piece[TETROMINO_SIZE][TETROMINO_SIZE];
extern int J_piece[TETROMINO_SIZE][TETROMINO_SIZE];
extern int L_piece[TETROMINO_SIZE][TETROMINO_SIZE];

int (*getPieceByType(TetrominoType type))[TETROMINO_SIZE];

// RGB color struct and lookup
struct Color { Uint8 r, g, b; };
Color getPieceColor(TetrominoType type);

#endif //PIECES_H
