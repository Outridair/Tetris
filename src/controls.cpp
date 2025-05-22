//
// Created by David Rynearson on 18.05.25.
//
#include "../include/controls.h"
#include <iostream>
#include "../include/collision.h"

void handleInput(SDL_Event& event, int& pieceX, int& pieceY, int board[20][10], int (*piece)[4], int cols) {
    std::cout << "Handling input..." << std::endl;
    if (event.type == SDL_KEYDOWN) {
        std::cout << "Key pressed: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl;

        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                if (canMove(pieceX - 1, pieceY, piece, board)) pieceX--;
                break;
            case SDLK_RIGHT:
                if (canMove(pieceX + 1, pieceY, piece, board)) pieceX++;
                break;
            case SDLK_DOWN:
                if (canMove(pieceX, pieceY + 1, piece, board)) pieceY++;
                break;
        }
    }
}
