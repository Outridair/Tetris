//
// Created by David Rynearson on 27.05.25.
//

#include "../include/Renderer.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

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

void Renderer::drawScore(int score, int level, int lines) {
    std::string txt = "Score: " + std::to_string(score)
                    + "\nLevel: " + std::to_string(level)
                    + "\nLines: " + std::to_string(lines);
    SDL_Color white = {255, 255, 255, 255 };
    SDL_Surface* surf = TTF_RenderText_Blended_Wrapped(font, txt.c_str(), white, 200);
    if (!surf) return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
    SDL_Rect dst = { offsetX + BOARD_PIX_W + 20, offsetY, surf->w, surf->h};
    SDL_FreeSurface(surf);
    if (!tex) return;

    SDL_RenderCopy(ren, tex, nullptr, &dst);
    SDL_DestroyTexture(tex);
}

void Renderer::drawNextPiece(Tetromino::Type nextType) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* labelSurf = TTF_RenderText_Blended(font, "Next", white);
    SDL_Texture* labelTex = SDL_CreateTextureFromSurface(ren, labelSurf);
    int labelW = labelSurf->w;
    int labelH = labelSurf->h;
    SDL_FreeSurface(labelSurf);

    constexpr int gridSize = 4 * CELL_SIZE;

    constexpr int gap = 10;  // px between label and grid
    int innerW = std::max(labelW, gridSize);
    int innerH = labelH + gap + gridSize;

    constexpr int padding = 8;  // px of space inside the border
    constexpr int margin  = 20; // px between board and preview-border


    int borderW = innerW + 2 * padding;
    int borderH = innerH + 2 * padding;

    int borderX = offsetX - margin - borderW;
    int borderY = offsetY;  // align top of border with top of board

    SDL_Rect borderRect { borderX, borderY, borderW, borderH };

    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 128); // half-transparent background
    SDL_RenderFillRect(ren, &borderRect);

    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderDrawRect(ren, &borderRect);

    int labelDstX = borderX + padding + (innerW - labelW) / 2;
    int labelDstY = borderY + padding;
    SDL_Rect labelDst { labelDstX, labelDstY, labelW, labelH };
    SDL_RenderCopy(ren, labelTex, nullptr, &labelDst);
    SDL_DestroyTexture(labelTex);

    int gridStartX = borderX + padding + (innerW - gridSize) / 2;
    int gridStartY = labelDstY + labelH + gap;

    Tetromino preview(nextType);
    const auto& shape = preview.currentShape();
    SDL_Color color   = preview.getColor();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (shape[i][j]) {
                SDL_Rect cellRect {
                    gridStartX + j * CELL_SIZE,
                    gridStartY + i * CELL_SIZE,
                    CELL_SIZE,
                    CELL_SIZE
                };
                // fill cell
                SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);
                SDL_RenderFillRect(ren, &cellRect);

                // draw a thin dark border around each cell
                SDL_SetRenderDrawColor(ren, 50, 50, 50, 255);
                SDL_RenderDrawRect(ren, &cellRect);
            }
        }
    }
}
