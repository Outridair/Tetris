//
// Created by David Rynearson on 27.05.25.
//

#ifndef TETROMINO_HPP
#define TETROMINO_HPP
#include <array>
#include <SDL_pixels.h>

using Shape = std::array<std::array<int, 4>,4>;

class Tetromino {
public:
    enum Type {I, O, T, S, Z, J, L};
    explicit Tetromino(Type t);
    [[nodiscard]] Type getType() const { return type; }
    [[nodiscard]] const Shape& currentShape() const;
    void rotateCW();
    void rotateCCW();
    [[nodiscard]] SDL_Color getColor() const;

    int x, y;

private:
    Type type;
    int rotationIndex;
    std::array<Shape, 4> rotations{};
    void initRotations();
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
