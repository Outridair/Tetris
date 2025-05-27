//
// Created by David Rynearson on 26.05.25.
//

#ifndef BOARD_HPP
#define BOARD_HPP
#include <array>

static const int BOARD_WIDTH = 10;
static const int BOARD_HEIGHT = 20;

class Board {
public:
    Board();
    [[nodiscard]] bool isOccupied(int x, int y) const;
    void occupy(int x, int y);
    int clearFullLines();

private:
    std::array<std::array<int, BOARD_WIDTH>, BOARD_HEIGHT> grid;
};



#endif //BOARD_HPP
