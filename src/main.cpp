#include <SDL.h>
#include <iostream>
#include "../include/controls.h"
#include "../include/pieces.h"
#include <cstdlib>
#include <ctime>
#include "../include/collision.h"

void spawnNewPiece(TetrominoType& type, int (*&piece)[TETROMINO_SIZE], int& pieceX, int& pieceY) {
    type = static_cast<TetrominoType>(std::rand() % 7);
    piece = getPieceByType(type);
    pieceX = 4;
    pieceY = 0;
}

int main(int argc, char* argv[]) {
    //Main Window Loop
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Tetris SDL2",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720, SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }

    // PlayGrid
    const int cellSize = 24;
    const int cols = 10;
    const int rows = 20;
    const int gridX = 100;
    const int gridY = 40;
    int gameBoard[20][10] = {0};
    std::srand(std::time(nullptr));


    //Square Piece
    int pieceX = 4;
    int pieceY = 0;
    int fallTimer = 0;

    TetrominoType currentType = O;
    int(*currentPiece)[TETROMINO_SIZE] = nullptr;
    spawnNewPiece(currentType, currentPiece, pieceX, pieceY);

    SDL_Event e;
    bool running = true;


    //Game Loop
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            handleInput(e, pieceX, pieceY, gameBoard, currentPiece, cols);
        }



        SDL_SetRenderDrawColor(renderer, 100, 100,100, 255);
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                if (gameBoard[row][col]) {
                    SDL_Rect cell = { gridX + col*cellSize, gridY + row*cellSize, cellSize-1, cellSize-1 };
                    SDL_RenderFillRect(renderer, &cell);
                }
                SDL_Rect cell;
                cell.x = gridX + col * cellSize;
                cell.y = gridY + row * cellSize;
                cell.w = cellSize - 1;
                cell.h = cellSize - 1;

                SDL_RenderDrawRect(renderer, &cell);
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);

        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); //Yellow
        for (int r = 0; r < TETROMINO_SIZE; ++r) {
            for (int c = 0; c < TETROMINO_SIZE; ++c) {
                if (currentPiece[r][c]) {
                    SDL_Rect cell = {
                        gridX + (pieceX + c) * cellSize,
                        gridY + (pieceY + r) * cellSize,
                        cellSize - 1,
                        cellSize - 1
                    };
                    SDL_RenderFillRect(renderer, &cell);
                }
            }
        }

        fallTimer++;
        if (fallTimer > 30) {
            if (canMove(pieceX, pieceY + 1, currentPiece, gameBoard)) {
                pieceY++;
            } else {
                // Lock into board
                for (int row = 0; row < TETROMINO_SIZE; ++row) {
                    for (int col = 0; col < TETROMINO_SIZE; ++col) {
                        if (currentPiece[row][col]) {
                            int x = pieceX + col;
                            int y = pieceY + row;
                            if (y >= 0 && y < rows && x >= 0 && x < cols) {
                                gameBoard[y][x] = 1;
                            }
                        }
                    }
                }
                // Spawn next random piece
                spawnNewPiece(currentType, currentPiece, pieceX, pieceY);
            }

            fallTimer = 0;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
