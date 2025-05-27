//
// Created by David Rynearson on 27.05.25.
//

#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <SDL.h>
#include "Board.hpp"
#include "Tetromino.hpp"


class Renderer {
public:
    Renderer(SDL_Renderer* ren) : ren(ren) {}
    void drawBoard(const Board& board);
    void drawTetromino(const Tetromino& t);

private:
    SDL_Renderer* ren;
    void drawCell(int x, int y, SDL_Color color);
};



#endif //RENDERER_HPP
