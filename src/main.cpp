#include <SDL.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "../include/controls.h"
#include "../include/pieces.h"
#include "../include/collision.h"

// Constants
constexpr int SCREEN_W     = 1280;
constexpr int SCREEN_H     = 720;
constexpr int GRID_COLS    = 10;
constexpr int GRID_ROWS    = 20;
constexpr int CELL_SIZE    = 24;
constexpr int GRID_X       = 100;
constexpr int GRID_Y       = 40;
constexpr int DROP_DELAY   = 80;  // frames between automatic falls

// Game state
SDL_Window*   gWindow   = nullptr;
SDL_Renderer* gRenderer = nullptr;
int           gameBoard[GRID_ROWS][GRID_COLS] = {};
TetrominoType currentType;
int          (*currentPiece)[TETROMINO_SIZE] = nullptr;
int           pieceX = GRID_COLS / 2;
int           pieceY = 0;
int           fallTimer = 0;
bool          running = true;

//----------------------------------------------------------------
// Forward declarations
//----------------------------------------------------------------
bool initSDL();
void shutdownSDL();
void spawnNewPiece();
void handleEvents();
void updateGame();
void renderFrame();

//----------------------------------------------------------------
// Entry point
//----------------------------------------------------------------
int main(int, char*[])
{
    if (!initSDL()) return 1;

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    spawnNewPiece();

    // Main loop
    while (running) {
        handleEvents();
        updateGame();
        renderFrame();
        SDL_Delay(16);
    }

    shutdownSDL();
    return 0;
}

//----------------------------------------------------------------
// Initialize SDL, window, renderer
//----------------------------------------------------------------
bool initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL init failed: " << SDL_GetError() << "\n";
        return false;
    }
    gWindow = SDL_CreateWindow(
        "Tetris SDL2",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN
    );
    if (!gWindow) {
        std::cerr << "Window creation failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return false;
    }
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!gRenderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(gWindow);
        SDL_Quit();
        return false;
    }
    return true;
}

//----------------------------------------------------------------
// Clean up SDL resources
//----------------------------------------------------------------
void shutdownSDL()
{
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

//----------------------------------------------------------------
// Choose a new random piece and reset position/timer
//----------------------------------------------------------------
void spawnNewPiece()
{
    currentType  = static_cast<TetrominoType>(std::rand() % 7);
    currentPiece = getPieceByType(currentType);
    pieceX       = GRID_COLS / 2 - TETROMINO_SIZE / 2;
    pieceY       = 0;
    fallTimer    = 0;
}

//----------------------------------------------------------------
// Process all pending SDL events
//----------------------------------------------------------------
void handleEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        }
        else {
            handleInput(e, pieceX, pieceY, gameBoard, currentPiece, GRID_COLS);
        }
    }
}

//----------------------------------------------------------------
// Update game logic: gravity, collision, locking, line clear
//----------------------------------------------------------------
void updateGame()
{
    fallTimer++;
    if (fallTimer <= DROP_DELAY) return;

    if (canMove(pieceX, pieceY + 1, currentPiece, gameBoard)) {
        pieceY++;
    }
    else {
        // Lock piece into board
        for (int r = 0; r < TETROMINO_SIZE; ++r) {
            for (int c = 0; c < TETROMINO_SIZE; ++c) {
                if (currentPiece[r][c]) {
                    int x = pieceX + c;
                    int y = pieceY + r;
                    if (y >= 0 && y < GRID_ROWS && x >= 0 && x < GRID_COLS) {
                        gameBoard[y][x] = static_cast<int>(currentType) + 1;
                    }
                }
            }
        }
        // TODO: clear full lines here

        spawnNewPiece();
    }
}

//----------------------------------------------------------------
// Render grid, locked blocks, and active piece, then present
//----------------------------------------------------------------
void renderFrame()
{
    // 1) clear screen
    SDL_SetRenderDrawColor(gRenderer, 100, 100, 100, 255);
    SDL_RenderClear(gRenderer);

    // 2) draw grid lines
    SDL_SetRenderDrawColor(gRenderer, 50, 50, 50, 255);
    for (int r = 0; r < GRID_ROWS; ++r) {
        for (int c = 0; c < GRID_COLS; ++c) {
            SDL_Rect cell { GRID_X + c*CELL_SIZE,
                            GRID_Y + r*CELL_SIZE,
                            CELL_SIZE - 1,
                            CELL_SIZE - 1 };
            SDL_RenderDrawRect(gRenderer, &cell);
        }
    }

    // 3) draw locked blocks
    for (int r = 0; r < GRID_ROWS; ++r) {
        for (int c = 0; c < GRID_COLS; ++c) {
            int val = gameBoard[r][c];
            if (val) {
                Color col = getPieceColor(static_cast<TetrominoType>(c - 1));
                SDL_SetRenderDrawColor(gRenderer, col.r, col.g, col.b, 255);
                SDL_Rect block { GRID_X + c*CELL_SIZE,
                                 GRID_Y + r*CELL_SIZE,
                                 CELL_SIZE - 1,
                                 CELL_SIZE - 1 };
                SDL_RenderFillRect(gRenderer, &block);
            }
        }
    }

    // 4) draw active piece
    Color active = getPieceColor(currentType);
    SDL_SetRenderDrawColor(gRenderer, active.r, active.g, active.b, 255);
    for (int r = 0; r < TETROMINO_SIZE; ++r) {
        for (int c = 0; c < TETROMINO_SIZE; ++c) {
            if (currentPiece[r][c]) {
                SDL_Rect block { GRID_X + (pieceX + c)*CELL_SIZE,
                                 GRID_Y + (pieceY + r)*CELL_SIZE,
                                 CELL_SIZE - 1,
                                 CELL_SIZE - 1 };
                SDL_RenderFillRect(gRenderer, &block);
            }
        }
    }

    // 5) present to screen
    SDL_RenderPresent(gRenderer);
}
