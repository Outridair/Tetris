//
// Created by David Rynearson on 27.05.25.
//

#ifndef TETROMINO_HPP
#define TETROMINO_HPP
#include <array>
#include <SDL_pixels.h>

class Board;
using Shape = std::array<std::array<int, 4>,4>;

class Tetromino {
public:
    enum Type : int { I, O, T, S, Z, J, L, EMPTY };
    explicit Tetromino(Type t);
    [[nodiscard]] Type getType() const { return type; }
    [[nodiscard]] const Shape& currentShape() const;
    bool tryRotateCW(const Board& board);
    bool tryRotateCCW(const Board& board);
    [[nodiscard]] SDL_Color getColor() const;

    int x, y;

private:
    Type type;
    int rotationIndex;
    std::array<Shape, 4> rotations{};
    void initRotations();
    bool collides(const Board& board, int testX, int testY, int testRot) const;
};

// 2) Now that Tetromino::Type is in scope, we can define the name‐array:
static constexpr const char* TetrominoNames[] = {
    "I", "O", "T", "S", "Z", "J", "L"
};

// 3) And the inline helper that maps Type → string:
inline const char* toString(Tetromino::Type t) {
    return TetrominoNames[static_cast<int>(t)];
}



#endif //TETROMINO_HPP
