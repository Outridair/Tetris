#include "../include/Game.hpp"

int main(int argc, char* argv[]) {
    Game game;
    std::srand(unsigned(std::time(nullptr)));  // seed once in practice—moved here for clarity
    if (!game.init("Tetris", 800, 800))
        return -1;
    game.run();
    return 0;
}