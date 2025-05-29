//
// Created by David Rynearson on 27.05.25.
//

#include "../include/Tetromino.hpp"
#include "../include/Board.hpp"
#include <SDL.h>
#include <vector>

static const std::vector<std::pair<int,int>> kickOffsets = {
    {0, 0},
    {-1, 0},
    {+1, 0},
    {0, -1},
    {0, +1},
};

Tetromino::Tetromino(Type t)
    : type(t), rotationIndex(0), x(BOARD_WIDTH/2 - 2), y(0) {
    initRotations();
}

void Tetromino::initRotations() {
    Shape base{};
    switch (type) {
        case EMPTY:
            break;
        case I:
            base = {{
                {{0,0,0,0}},
                {{1,1,1,1}},
                {{0,0,0,0}},
                {{0,0,0,0}}
            }};
            break;
        case O:
            base = {{
                {{0,0,0,0}},
                {{0,1,1,0}},
                {{0,1,1,0}},
                {{0,0,0,0}}
            }};
            for (int i = 0; i < 4; ++i) {
                rotations[i] = base;
            }
            break;
        case T:
            base = {{
                {{0,1,0,0}},
                {{1,1,1,0}},
                {{0,0,0,0}},
                {{0,0,0,0}}
            }};
            break;
        case S:
            base = {{
                {{0,0,0,0}},
                {{0,1,1,0}},
                {{1,1,0,0}},
                {{0,0,0,0}}
            }};
            break;
        case Z:
            base = {{
                {{0,0,0,0}},
                {{1,1,0,0}},
                {{0,1,1,0}},
                {{0,0,0,0}}
            }};
            break;
        case J:
            base = {{
                {{0,0,0,0}},
                {{1,0,0,0}},
                {{1,1,1,0}},
                {{0,0,0,0}}
            }};
            break;
        case L:
            base = {{
                {{0,0,0,0}},
                {{0,0,1,0}},
                {{1,1,1,0}},
                {{0,0,0,0}}
            }};
            break;
    }
    rotations[0] = base;

    // Rotate shape clockwise
    auto rotateShape = [](const Shape& s) {
        Shape r{};
        for (int i =0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                r[j][3-i] = s[i][j];
            }
        }
        return r;
    };

    for (int i = 1; i <4; ++i) {
        rotations[i] = rotateShape(rotations[i-1]);
    }
}

const Shape &Tetromino::currentShape() const {
    return rotations[rotationIndex];
}

bool Tetromino::tryRotateCW(const Board& board) {
    int newRot = (rotationIndex + 1) % 4;
    if (type == O) {
        rotationIndex = newRot;
        return true;
    }
    for (auto [dx, dy] : kickOffsets) {
        if (!collides(board, x + dx, y + dy, newRot)) {
            rotationIndex = newRot;
            x += dx;
            y += dy;
            return true;
        }
    }
    return false;
}

bool Tetromino::tryRotateCCW(const Board& board) {
    int newRot = (rotationIndex + 3) % 4;
    if (type == O) {
        rotationIndex = newRot;
        return true;
    }
    for (auto [dx, dy] : kickOffsets) {
        if (!collides(board, x + dx, y + dy, newRot)) {
            rotationIndex = newRot;
            x += dx;
            y += dy;
            return true;
        }
    }
    return false;
}

SDL_Color Tetromino::getColor() const {
    switch (type) {
        case I: return SDL_Color{0, 255, 255, 255};   // Cyan
        case O: return SDL_Color{255, 255, 0, 255};   // Yellow
        case T: return SDL_Color{128,   0, 128, 255}; // Purple
        case S: return SDL_Color{0, 255,   0, 255};   // Green
        case Z: return SDL_Color{255,   0,   0, 255}; // Red
        case J: return SDL_Color{0,   0, 255, 255};   // Blue
        case L: return SDL_Color{255, 165,   0, 255}; // Orange
        default: return SDL_Color{200, 200, 200, 255};
    }
}

bool Tetromino::collides(const Board& board,
                         int testX, int testY,
                         int testRot) const
{
    const auto& shape = rotations[testRot];
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (!shape[i][j]) continue;
            if (board.isOccupied(testX + j, testY + i))
                return true;
        }
    }
    return false;
}

