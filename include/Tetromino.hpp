//
// Created by David Rynearson on 27.05.25.
//

#ifndef TETROMINO_HPP
#define TETROMINO_HPP
#include <array>

using Shape = std::array<std::array<int, 4>,4>;


class Tetromino {
public:
    enum Type {I, O, T, S, Z, J, L};
    explicit Tetromino(Type t);

    [[nodiscard]] const Shape& currentShape() const;
    void rotateCW();
    void rotateCCW();

    int x, y;

private:
    Type type;
    int rotationIndex;
    std::array<Shape, 4> rotations;
    void initRotations();
};



#endif //TETROMINO_HPP
