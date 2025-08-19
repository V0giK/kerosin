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

#pragma once

#include "lovyanGfxSetup.h"
#include "main.h"
#include <lvgl.h>
#include <vector>
#include <algorithm>

// Game constants
#define AERO_GRID_SIZE 30       // Size of each block in pixels
#define AERO_BOARD_WIDTH 10     // Width of the game board in blocks
#define AERO_BOARD_HEIGHT 20    // Height of the game board in blocks
#define AERO_NEXT_PIECE_SIZE 4  // Size of the next piece preview
#define AERO_MAX_LEVEL 10       // Maximum game level
#define AERO_HIGHSCORE_COUNT 5  // Number of high scores to store

// Piece definitions - using aviation themes where each shape represents a different aircraft type
// Each piece is defined as a 4x4 grid with bits representing filled cells
enum AeroPieceType {
    AERO_FIGHTER = 0,  // I piece (fighter jet - long and straight)
    AERO_BOMBER,       // J piece (bomber - heavy, leans left)
    AERO_STEALTH,      // L piece (stealth aircraft - angular, leans right)
    AERO_CARGO,        // O piece (cargo plane - wide and stable)
    AERO_HELICOPTER,   // S piece (helicopter - curved shape)
    AERO_SEAPLANE,     // Z piece (seaplane - curved in other direction)
    AERO_BIPLANE,      // T piece (biplane - T-shaped classic)
    AERO_PIECE_COUNT   // Total number of piece types
};

// Define colors for each piece type
#define AERO_COLOR_FIGHTER      lv_color_hex(0x00FFFF) // Cyan for fighter jets
#define AERO_COLOR_BOMBER       lv_color_hex(0x0000FF) // Blue for bombers
#define AERO_COLOR_STEALTH      lv_color_hex(0xFF7F00) // Orange for stealth
#define AERO_COLOR_CARGO        lv_color_hex(0xFFFF00) // Yellow for cargo
#define AERO_COLOR_HELICOPTER   lv_color_hex(0x00FF00) // Green for helicopter
#define AERO_COLOR_SEAPLANE     lv_color_hex(0xFF0000) // Red for seaplane
#define AERO_COLOR_BIPLANE      lv_color_hex(0x7F00FF) // Purple for biplane
#define AERO_COLOR_BACKGROUND   lv_color_hex(0x1A1A3A) // Dark blue sky background
#define AERO_COLOR_GRID         lv_color_hex(0x3A3A5A) // Lighter grid lines
#define AERO_COLOR_TEXT         lv_color_hex(0xFFFFFF) // White text
#define AERO_COLOR_CLOUDS       lv_color_hex(0xDDDDFF) // Light blue clouds
#define AERO_COLOR_BUTTON       lv_color_hex(0x4444AA) // Button color
#define AERO_COLOR_BUTTON_PRESS lv_color_hex(0x6666CC) // Button press color

// Game state enum
enum AeroGameState {
    AERO_STATE_TITLE,
    AERO_STATE_PLAYING,
    AERO_STATE_PAUSED,
    AERO_STATE_GAME_OVER,
    AERO_STATE_HIGH_SCORES
};

// Structure for high score entries
struct AeroHighScore {
    int score;
    int level;
    int lines;
    char name[10];
    
    // Constructor for initialization
    AeroHighScore(int s = 0, int lvl = 0, int ln = 0, const char* n = "PILOT") : 
        score(s), level(lvl), lines(ln) {
        strncpy(name, n, 9);
        name[9] = '\0';
    }
    
    // Comparison operator for sorting
    bool operator<(const AeroHighScore& other) const {
        return score > other.score; // Sort in descending order
    }
};

class AeroBlocks {
public:
    AeroBlocks();
    ~AeroBlocks();

    // Game lifecycle methods
    void init();
    void start();
    void stop();
    void pause();
    void resume();
    void update();
    
    // Check if game is currently active
    bool isRunning() const { return gameState == AERO_STATE_PLAYING; }

private:
    // Game state
    AeroGameState gameState;
    lv_obj_t* gameScreen;
    lv_obj_t* titleScreen;
    lv_obj_t* gameOverScreen;
    lv_obj_t* highScoreScreen;
    lv_obj_t* previousScreen;
    
    // Board and pieces
    uint8_t board[AERO_BOARD_HEIGHT][AERO_BOARD_WIDTH];
    lv_obj_t* boardObjects[AERO_BOARD_HEIGHT][AERO_BOARD_WIDTH];
    lv_obj_t* gameBoard;
    lv_obj_t* nextPiecePreview;
    
    // Current and next piece data
    int currentPieceType;
    int nextPieceType;
    int currentRotation;
    int currentX;
    int currentY;
    
    // Piece definitions
    const uint16_t pieceDefinitions[AERO_PIECE_COUNT][4];
    
    // Score and timing
    lv_obj_t* scoreLabel;
    lv_obj_t* levelLabel;
    lv_obj_t* linesLabel;
    int score;
    int level;
    int lines;
    int linesForNextLevel;
    uint32_t lastDropTime;
    uint32_t dropInterval;
    uint32_t gameStartTime;
    
    // Touch controls
    lv_obj_t* leftButton;
    lv_obj_t* rightButton;
    lv_obj_t* rotateButton;
    lv_obj_t* dropButton;
    lv_obj_t* pauseButton;
    
    // High scores
    std::vector<AeroHighScore> highScores;
    
    // Game methods
    void createTitleScreen();
    void createGameScreen();
    void createGameOverScreen();
    void createHighScoreScreen();
    
    void resetGame();
    void generateNextPiece();
    void drawBoard();
    void updatePiecePreview();
    void updateScoreDisplay();
    void checkForCompletedLines();
    bool isCollision(int pieceType, int rotation, int x, int y);
    void placePiece();
    void calculateDropInterval();
    void addHighScore();
    void saveHighScores();
    void loadHighScores();
    
    // Movement methods
    void moveLeft();
    void moveRight();
    void moveDown();
    void hardDrop();
    void rotate();
    
    // LVGL event callbacks
    static void buttonEventHandler(lv_event_t* e);
    static void screenEventHandler(lv_event_t* e);
    static void updateCallback(lv_timer_t* timer);
    static void animationCallback(lv_timer_t* timer);
    
    // Helper methods
    lv_color_t getPieceColor(int pieceType);
    void drawPiece(int pieceType, int rotation, int x, int y, lv_obj_t* parent, bool preview = false);
    void clearPreview();
    void addCloudAnimation();
};
