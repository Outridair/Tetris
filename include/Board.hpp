//
// Created by David Rynearson on 26.05.25.
//

#ifndef BOARD_HPP
#define BOARD_HPP
#include <array>

#include "Tetromino.hpp"

static const int BOARD_WIDTH = 10;
static const int BOARD_HEIGHT = 20;

class Board {
public:
    Board() {
        for (auto& row : grid) row.fill(EMPTY);
    }
    static constexpr Tetromino::Type EMPTY = static_cast<Tetromino::Type>(-1);
    [[nodiscard]] bool isOccupied(int x, int y) const;
    void occupy(int x, int y, Tetromino::Type t);
    [[nodiscard]] Tetromino::Type getCellType(int x, int y) const;
    int clearFullLines();

private:
    std::array<std::array<Tetromino::Type, BOARD_WIDTH>, BOARD_HEIGHT> grid;
};



#endif //BOARD_HPP
