//
// Created by David Rynearson on 27.05.25.
//

#include "../include/Tetromino.hpp"
#include "../include/Board.hpp"

Tetromino::Tetromino(Type t)
    : type(t), rotationIndex(0), x(BOARD_WIDTH/2 - 2), y(0) {
    initRotations();
}

void Tetromino::initRotations() {
    Shape base{};
    switch (type) {
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
                {{0,1,1,0}},
                {{0,1,1,0}},
                {{0,0,0,0}},
                {{0,0,0,0}}
            }};
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
                {{0,1,1,0}},
                {{1,1,0,0}},
                {{0,0,0,0}},
                {{0,0,0,0}}
            }};
            break;
        case Z:
            base = {{
                {{1,1,0,0}},
                {{0,1,1,0}},
                {{0,0,0,0}},
                {{0,0,0,0}}
            }};
            break;
        case J:
            base = {{
                {{1,0,0,0}},
                {{1,1,1,0}},
                {{0,0,0,0}},
                {{0,0,0,0}}
            }};
            break;
        case L:
            base = {{
                {{0,0,1,0}},
                {{1,1,1,0}},
                {{0,0,0,0}},
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

void Tetromino::rotateCW() {
    rotationIndex = (rotationIndex + 1) % 4;
}

void Tetromino::rotateCCW() {
    rotationIndex = (rotationIndex + 3) % 4;
}


