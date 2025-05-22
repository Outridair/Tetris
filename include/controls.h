//
// Created by David Rynearson on 18.05.25.
//

#ifndef CONTROLS_H
#define CONTROLS_H
#include <SDL.h>

void handleInput(SDL_Event& event, int& pieceX, int& pieceY, int board[20][10], int (*piece)[4], int cols);

#endif //CONTROLS_H
