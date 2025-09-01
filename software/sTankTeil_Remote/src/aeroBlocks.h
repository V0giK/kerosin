/*
 * AeroBlocks - An aviation-themed Tetris game
 *
 * Copyright (C) 2025 V0giK
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef AEROBLOCKS_H
#define AEROBLOCKS_H

#include "lvgl.h"
#include "AeroHighScore.h"
#include <vector>

// Define screen dimensions
// #define SCREEN_WIDTH  480
// #define SCREEN_HEIGHT 320

// Define board dimensions
#define AERO_BOARD_WIDTH  10
#define AERO_BOARD_HEIGHT 20

// Define grid size for each cell in the board
#define AERO_GRID_SIZE  20

// Define number of different tetromino pieces
#define AERO_PIECE_COUNT 7

// Define next piece preview size
#define AERO_NEXT_PIECE_SIZE 4

// Game states
enum AeroGameState {
    AERO_STATE_TITLE,
    AERO_STATE_PLAYING,
    AERO_STATE_PAUSED,
    AERO_STATE_GAME_OVER
};

// Color definitions
#define AERO_COLOR_BACKGROUND lv_color_hex(0x000000)
#define AERO_COLOR_TEXT lv_color_hex(0xFFFFFF)
#define AERO_COLOR_BUTTON lv_color_hex(0x007BFF)
#define AERO_COLOR_GRID lv_color_hex(0xCCCCCC)
#define AERO_COLOR_FIGHTER lv_color_hex(0xFF0000)
#define AERO_COLOR_BOMBER lv_color_hex(0x00FF00)
#define AERO_COLOR_STEALTH lv_color_hex(0x0000FF)
#define AERO_COLOR_CARGO lv_color_hex(0xFFFF00)
#define AERO_COLOR_HELICOPTER lv_color_hex(0xFF00FF)
#define AERO_COLOR_SEAPLANE lv_color_hex(0x00FFFF)
#define AERO_COLOR_BIPLANE lv_color_hex(0xFFFFFF)
#define AERO_COLOR_CLOUDS lv_color_hex(0xAAAAAA)

// Piece type enums for use in switch/case and logic
enum AeroPieceType {
    AERO_FIGHTER = 0,
    AERO_BOMBER,
    AERO_STEALTH,
    AERO_CARGO,
    AERO_HELICOPTER,
    AERO_SEAPLANE,
    AERO_BIPLANE
};

class AeroBlocks {
    public:
        AeroBlocks();
        ~AeroBlocks();
        
        // Initialization and start/stop methods
        void init();
        void start();
        void stop();
        
        // Game logic methods
        void resetGame();
        void update();
        void pause();
        void resume();
        
        // Drawing methods
        void drawBoard();
        void drawPiece(int pieceType, int rotation, int x, int y, lv_obj_t* parent, bool preview = false);
        
        // Preview and scoring methods
        void clearPreview();
        void updatePiecePreview();
        void updateScoreDisplay();
        int calculateScore(int linesCleared, int currentLevel);
        
        // Collision and movement methods
        bool checkCollision(int pieceType, int rotation, int x, int y);
        void moveLeft();
        void moveRight();
        void moveDown();
        void rotate();
        void hardDrop();
        
        // High score methods
        void loadHighScores();
        void saveHighScores();
        void createTitleScreen();
        static void buttonEventHandler(lv_event_t* e);
        void createHighScoreScreen();
        void createGameScreen();
        void createGameOverScreen(); // Game Over Screen anzeigen
        static void updateCallback(lv_timer_t* timer);
        lv_color_t getPieceColor(int pieceType);
        void addCloudAnimation();
        int clearCompletedLines();
        
    protected:
        AeroGameState gameState;
        lv_obj_t* gameScreen;
        lv_obj_t* titleScreen;
        lv_obj_t* gameOverScreen;
        lv_obj_t* highScoreScreen;
        lv_obj_t* previousScreen;
        lv_obj_t* gameBoard;
        lv_obj_t* nextPiecePreview;
        lv_obj_t* scoreLabel;
        lv_obj_t* levelLabel;
        lv_obj_t* linesLabel;
        lv_obj_t* leftButton;
        lv_obj_t* rightButton;
        lv_obj_t* rotateButton;
        lv_obj_t* dropButton;
        lv_obj_t* pauseButton;
        
        int currentPieceType;
        int nextPieceType;
        int currentRotation;
        int currentX;
        int currentY;
        
        uint16_t board[AERO_BOARD_HEIGHT][AERO_BOARD_WIDTH];
        lv_obj_t* boardObjects[AERO_BOARD_HEIGHT][AERO_BOARD_WIDTH];
        
        uint32_t lastDropTime;
        uint32_t dropInterval;
        
        int score;
        int level;
        int lines;
        int linesForNextLevel;
        
        uint16_t pieceDefinitions[AERO_PIECE_COUNT][4]; // Tetromino shapes
        uint32_t gameStartTime; // Startzeit des Spiels
        
        // Piece definitions - each represents a different aircraft type
        // Using standard tetromino shapes with aviation-themed names
        const uint16_t PIECE_DEFS[AERO_PIECE_COUNT][4] = {
            // AERO_FIGHTER (I-piece)
            {0x0F00, 0x2222, 0x00F0, 0x4444},
            // AERO_BOMBER (J-piece)
            {0x8E00, 0x6440, 0x0E20, 0x44C0},
            // AERO_STEALTH (L-piece)
            {0x2E00, 0x4460, 0x0E80, 0xC440},
            // AERO_CARGO (O-piece)
            {0x6600, 0x6600, 0x6600, 0x6600},
            // AERO_HELICOPTER (S-piece)
            {0x6C00, 0x4620, 0x06C0, 0x8C40},
            // AERO_SEAPLANE (Z-piece)
            {0xC600, 0x2640, 0x0C60, 0x4C80},
            // AERO_BIPLANE (T-piece)
            {0x4E00, 0x4640, 0x0E40, 0x4C40}
        };
        
        std::vector<AeroHighScore> highScores;
};

#endif