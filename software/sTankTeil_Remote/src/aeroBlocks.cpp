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

#include "aeroBlocks.h"
#include "main.h"
#include "esp_task_wdt.h"

// Debug output control
static const bool DEBUG = false;

// Tetris piece definitions - each represents a different aircraft type
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

AeroBlocks::AeroBlocks() : 
    gameState(AERO_STATE_TITLE),
    gameScreen(nullptr),
    titleScreen(nullptr),
    gameOverScreen(nullptr),
    highScoreScreen(nullptr),
    previousScreen(nullptr),
    gameBoard(nullptr),
    nextPiecePreview(nullptr),
    currentPieceType(0),
    nextPieceType(0),
    currentRotation(0),
    currentX(0),
    currentY(0),
    pieceDefinitions{
        {0x0F00, 0x2222, 0x00F0, 0x4444}, // AERO_FIGHTER (I-piece)
        {0x8E00, 0x6440, 0x0E20, 0x44C0}, // AERO_BOMBER (J-piece)
        {0x2E00, 0x4460, 0x0E80, 0xC440}, // AERO_STEALTH (L-piece)
        {0x6600, 0x6600, 0x6600, 0x6600}, // AERO_CARGO (O-piece)
        {0x6C00, 0x4620, 0x06C0, 0x8C40}, // AERO_HELICOPTER (S-piece)
        {0xC600, 0x2640, 0x0C60, 0x4C80}, // AERO_SEAPLANE (Z-piece)
        {0x4E00, 0x4640, 0x0E40, 0x4C40}  // AERO_BIPLANE (T-piece)
    },
    scoreLabel(nullptr),
    levelLabel(nullptr),
    linesLabel(nullptr),
    score(0),
    level(1),
    lines(0),
    linesForNextLevel(10),
    lastDropTime(0),
    dropInterval(1000),
    gameStartTime(0),
    leftButton(nullptr),
    rightButton(nullptr),
    rotateButton(nullptr),
    dropButton(nullptr),
    pauseButton(nullptr)
{
    // Initialize default high scores
    highScores.push_back(AeroHighScore(1000, 5, 20, "ACE"));
    highScores.push_back(AeroHighScore(800, 4, 16, "PILOT"));
    highScores.push_back(AeroHighScore(600, 3, 12, "CADET"));
    highScores.push_back(AeroHighScore(400, 2, 8, "ROOKIE"));
    highScores.push_back(AeroHighScore(200, 1, 4, "NOVICE"));
    
    if(DEBUG) Serial.println("AeroBlocks constructor called");
}

AeroBlocks::~AeroBlocks() {
    // Clean up resources to prevent memory leaks
    stop();
    if(DEBUG) Serial.println("AeroBlocks destructor called");
}

// Initialize game data
void AeroBlocks::init() {
    if(DEBUG) Serial.println("Initializing AeroBlocks game");
    
    // Reset the game board to empty
    memset(board, 0, sizeof(board));
    for (int y = 0; y < AERO_BOARD_HEIGHT; y++) {
        for (int x = 0; x < AERO_BOARD_WIDTH; x++) {
            boardObjects[y][x] = nullptr;
        }
    }
    
    // Initialize game variables
    score = 0;
    level = 1;
    lines = 0;
    linesForNextLevel = 10;
    dropInterval = 1000; // Start with 1 second drop interval
    gameState = AERO_STATE_TITLE;
    
    // Load high scores
    loadHighScores();
    
    if(DEBUG) Serial.println("AeroBlocks initialization complete");
}

// Start the game - entry point
void AeroBlocks::start() {
    if(DEBUG) Serial.println("Starting AeroBlocks game");
    
    // Reset watchdog to prevent timeout during initialization
    esp_task_wdt_reset();
    
    // Save current screen to return to later
    previousScreen = lv_scr_act();
    if (!previousScreen) {
        if(DEBUG) Serial.println("WARNING: No active screen to save");
    }
    
    // Create and display the title screen
    createTitleScreen();
}

// Stop the game and clean up resources
void AeroBlocks::stop() {
    if(DEBUG) Serial.println("Stopping AeroBlocks game");
    
    // Delete any active timers
    static lv_timer_t* game_timer = nullptr;
    if (game_timer) {
        lv_timer_del(game_timer);
        game_timer = nullptr;
    }
    
    // Clean up screen objects
    if (gameScreen) {
        lv_obj_del(gameScreen);
        gameScreen = nullptr;
    }
    
    if (titleScreen) {
        lv_obj_del(titleScreen);
        titleScreen = nullptr;
    }
    
    if (gameOverScreen) {
        lv_obj_del(gameOverScreen);
        gameOverScreen = nullptr;
    }
    
    if (highScoreScreen) {
        lv_obj_del(highScoreScreen);
        highScoreScreen = nullptr;
    }
    
    // Return to previous screen
    if (previousScreen) {
        lv_scr_load(previousScreen);
        previousScreen = nullptr;
    }
    
    // Reset game state
    gameState = AERO_STATE_TITLE;
    
    if(DEBUG) Serial.println("AeroBlocks stopped successfully");
}

// Create the title screen
void AeroBlocks::createTitleScreen() {
    if(DEBUG) Serial.println("Creating title screen");
    
    // Create a new screen for the title
    titleScreen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(titleScreen, AERO_COLOR_BACKGROUND, 0);
    
    // Create a stylish title label
    lv_obj_t* titleLabel = lv_label_create(titleScreen);
    lv_label_set_text(titleLabel, "AeroBlocks");
    lv_obj_set_style_text_color(titleLabel, AERO_COLOR_TEXT, 0);
    lv_obj_set_style_text_font(titleLabel, &lv_font_montserrat_30, 0);
    lv_obj_align(titleLabel, LV_ALIGN_TOP_MID, 0, 40);
    
    // Add a subtitle
    lv_obj_t* subtitleLabel = lv_label_create(titleScreen);
    lv_label_set_text(subtitleLabel, "Aviation Tetris Challenge");
    lv_obj_set_style_text_color(subtitleLabel, AERO_COLOR_CLOUDS, 0);
    lv_obj_set_style_text_font(subtitleLabel, &lv_font_montserrat_16, 0);
    lv_obj_align(subtitleLabel, LV_ALIGN_TOP_MID, 0, 80);
    
    // Add exit button in top-left corner
    lv_obj_t* exitButton = lv_btn_create(titleScreen);
    lv_obj_set_size(exitButton, 60, 40);
    lv_obj_set_pos(exitButton, 10, 10);
    lv_obj_set_style_bg_color(exitButton, AERO_COLOR_BUTTON, 0);
    lv_obj_add_event_cb(exitButton, [](lv_event_t* e) {
        AeroBlocks* game = static_cast<AeroBlocks*>(lv_event_get_user_data(e));
        game->stop();
    }, LV_EVENT_CLICKED, this);
    
    lv_obj_t* exitLabel = lv_label_create(exitButton);
    lv_label_set_text(exitLabel, LV_SYMBOL_CLOSE);
    lv_obj_center(exitLabel);
    
    // Add plane silhouette images or decorative elements
    // We'll use shapes as placeholders for aircraft silhouettes
    lv_obj_t* planeDecor1 = lv_obj_create(titleScreen);
    lv_obj_set_size(planeDecor1, 100, 20);
    lv_obj_set_style_radius(planeDecor1, 10, 0);
    lv_obj_set_style_bg_color(planeDecor1, AERO_COLOR_FIGHTER, 0);
    lv_obj_align(planeDecor1, LV_ALIGN_LEFT_MID, 40, -60);
    lv_obj_set_style_transform_angle(planeDecor1, 300, 0);
    
    lv_obj_t* planeDecor2 = lv_obj_create(titleScreen);
    lv_obj_set_size(planeDecor2, 100, 20);
    lv_obj_set_style_radius(planeDecor2, 10, 0);
    lv_obj_set_style_bg_color(planeDecor2, AERO_COLOR_BOMBER, 0);
    lv_obj_align(planeDecor2, LV_ALIGN_RIGHT_MID, -40, 60);
    lv_obj_set_style_transform_angle(planeDecor2, 60, 0);
    
    // Create start button
    lv_obj_t* startBtn = lv_btn_create(titleScreen);
    lv_obj_set_size(startBtn, 200, 60);
    lv_obj_set_style_bg_color(startBtn, AERO_COLOR_BUTTON, 0);
    lv_obj_align(startBtn, LV_ALIGN_CENTER, 0, 40);
    lv_obj_add_event_cb(startBtn, buttonEventHandler, LV_EVENT_CLICKED, this);
    lv_obj_set_user_data(startBtn, (void*)1); // 1 = start game
    
    lv_obj_t* startLabel = lv_label_create(startBtn);
    lv_label_set_text(startLabel, "Start Game");
    lv_obj_center(startLabel);
    
    // Create high scores button
    lv_obj_t* highScoreBtn = lv_btn_create(titleScreen);
    lv_obj_set_size(highScoreBtn, 200, 60);
    lv_obj_set_style_bg_color(highScoreBtn, AERO_COLOR_BUTTON, 0);
    lv_obj_align(highScoreBtn, LV_ALIGN_CENTER, 0, 120);
    lv_obj_add_event_cb(highScoreBtn, buttonEventHandler, LV_EVENT_CLICKED, this);
    lv_obj_set_user_data(highScoreBtn, (void*)2); // 2 = high scores
    
    lv_obj_t* highScoreLabel = lv_label_create(highScoreBtn);
    lv_label_set_text(highScoreLabel, "High Scores");
    lv_obj_center(highScoreLabel);
    
    // Create "cloud" decorations in the background
    for (int i = 0; i < 5; i++) {
        lv_obj_t* cloud = lv_obj_create(titleScreen);
        lv_obj_set_size(cloud, 40 + (i * 20), 20 + (i % 3) * 10);
        lv_obj_set_style_radius(cloud, 20, 0);
        lv_obj_set_style_bg_color(cloud, AERO_COLOR_CLOUDS, 0);
        lv_obj_set_style_bg_opa(cloud, 160, 0);
        lv_obj_set_style_border_width(cloud, 0, 0);
        
        // Position clouds randomly
        int x = 50 + (i * 150) % (SCREEN_WIDTH - 100);
        int y = 50 + (i * 70) % (SCREEN_HEIGHT - 200);
        lv_obj_set_pos(cloud, x, y);
    }
    
    // Load the title screen
    lv_scr_load(titleScreen);
    
    if(DEBUG) Serial.println("Title screen created");
}

// Handle button events
void AeroBlocks::buttonEventHandler(lv_event_t* e) {
    // Get the AeroBlocks instance from user data
    AeroBlocks* game = static_cast<AeroBlocks*>(lv_event_get_user_data(e));
    lv_obj_t* btn = (lv_obj_t*)lv_event_get_current_target(e); // Fix: Added explicit cast to lv_obj_t*
    int userData = (int)(uintptr_t)lv_obj_get_user_data(btn);
    
    if(DEBUG) Serial.printf("Button event: %d\n", userData);
    
    switch (userData) {
        case 1: // Start Game button
            if(DEBUG) Serial.println("Start Game button pressed");
            if (game->titleScreen) {
                lv_obj_del(game->titleScreen);
                game->titleScreen = nullptr;
            }
            game->resetGame();
            game->createGameScreen();
            break;
            
        case 2: // High Scores button
            if(DEBUG) Serial.println("High Scores button pressed");
            if (game->titleScreen) {
                lv_obj_del(game->titleScreen);
                game->titleScreen = nullptr;
            }
            game->createHighScoreScreen();
            break;
            
        default:
            if(DEBUG) Serial.printf("Unknown button ID: %d\n", userData);
            break;
    }
}

// Create a placeholder method for the high scores screen
void AeroBlocks::createHighScoreScreen() {
    if(DEBUG) Serial.println("Creating high score screen (placeholder)");
    
    // TODO: Implement full high score screen in next development phase
    
    // Create simple high score screen for now
    highScoreScreen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(highScoreScreen, AERO_COLOR_BACKGROUND, 0);
    
    // Create title
    lv_obj_t* titleLabel = lv_label_create(highScoreScreen);
    lv_label_set_text(titleLabel, "High Scores");
    lv_obj_set_style_text_color(titleLabel, AERO_COLOR_TEXT, 0);
    lv_obj_set_style_text_font(titleLabel, &lv_font_montserrat_30, 0);
    lv_obj_align(titleLabel, LV_ALIGN_TOP_MID, 0, 40);
    
    // Create back button
    lv_obj_t* backBtn = lv_btn_create(highScoreScreen);
    lv_obj_set_size(backBtn, 100, 50);
    lv_obj_set_style_bg_color(backBtn, AERO_COLOR_BUTTON, 0);
    lv_obj_align(backBtn, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_add_event_cb(backBtn, [](lv_event_t* e){
        AeroBlocks* game = static_cast<AeroBlocks*>(lv_event_get_user_data(e));
        if (game->highScoreScreen) {
            lv_obj_del(game->highScoreScreen);
            game->highScoreScreen = nullptr;
        }
        game->createTitleScreen();
    }, LV_EVENT_CLICKED, this);
    
    lv_obj_t* backLabel = lv_label_create(backBtn);
    lv_label_set_text(backLabel, "Back");
    lv_obj_center(backLabel);
    
    // Load high score screen
    lv_scr_load(highScoreScreen);
}

// Create proper game screen with board, controls and UI elements
void AeroBlocks::createGameScreen() {
    if(DEBUG) Serial.println("Creating game screen");
    
    // Create main game screen
    gameScreen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(gameScreen, AERO_COLOR_BACKGROUND, 0);
    
    // Calculate board dimensions and position
    int boardWidth = AERO_GRID_SIZE * AERO_BOARD_WIDTH;
    int boardHeight = AERO_GRID_SIZE * AERO_BOARD_HEIGHT;
    int boardX = (SCREEN_WIDTH - boardWidth) / 2 - 100; // Offset to left to make room for info panel
    int boardY = (SCREEN_HEIGHT - boardHeight) / 2;
    
    // Create the game board container
    gameBoard = lv_obj_create(gameScreen);
    lv_obj_set_size(gameBoard, boardWidth, boardHeight);
    lv_obj_set_pos(gameBoard, boardX, boardY);
    lv_obj_set_style_bg_color(gameBoard, AERO_COLOR_BACKGROUND, 0);
    lv_obj_set_style_border_color(gameBoard, AERO_COLOR_GRID, 0);
    lv_obj_set_style_border_width(gameBoard, 2, 0);
    
    // Create info panel
    lv_obj_t* infoPanel = lv_obj_create(gameScreen);
    lv_obj_set_size(infoPanel, 180, boardHeight);
    lv_obj_set_pos(infoPanel, boardX + boardWidth + 20, boardY);
    lv_obj_set_style_bg_color(infoPanel, lv_color_darken(AERO_COLOR_BACKGROUND, 20), 0);
    lv_obj_set_style_radius(infoPanel, 10, 0);
    lv_obj_set_style_border_width(infoPanel, 2, 0);
    lv_obj_set_style_border_color(infoPanel, AERO_COLOR_GRID, 0);
    
    // Create next piece preview panel
    lv_obj_t* previewLabel = lv_label_create(infoPanel);
    lv_label_set_text(previewLabel, "NEXT");
    lv_obj_set_style_text_color(previewLabel, AERO_COLOR_TEXT, 0);
    lv_obj_align(previewLabel, LV_ALIGN_TOP_MID, 0, 10);
    
    // Next piece preview container
    nextPiecePreview = lv_obj_create(infoPanel);
    int previewSize = AERO_GRID_SIZE * AERO_NEXT_PIECE_SIZE;
    lv_obj_set_size(nextPiecePreview, previewSize, previewSize);
    lv_obj_align(nextPiecePreview, LV_ALIGN_TOP_MID, 0, 40);
    lv_obj_set_style_bg_color(nextPiecePreview, lv_color_darken(AERO_COLOR_BACKGROUND, 10), 0);
    lv_obj_set_style_border_width(nextPiecePreview, 1, 0);
    lv_obj_set_style_border_color(nextPiecePreview, AERO_COLOR_GRID, 0);
    
    // Score, level, and lines display
    scoreLabel = lv_label_create(infoPanel);
    lv_label_set_text_fmt(scoreLabel, "SCORE\n%d", score);
    lv_obj_set_style_text_color(scoreLabel, AERO_COLOR_TEXT, 0);
    lv_obj_set_style_text_align(scoreLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(scoreLabel, LV_ALIGN_TOP_MID, 0, 150);
    
    levelLabel = lv_label_create(infoPanel);
    lv_label_set_text_fmt(levelLabel, "LEVEL\n%d", level);
    lv_obj_set_style_text_color(levelLabel, AERO_COLOR_TEXT, 0);
    lv_obj_set_style_text_align(levelLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(levelLabel, LV_ALIGN_TOP_MID, 0, 200);
    
    linesLabel = lv_label_create(infoPanel);
    lv_label_set_text_fmt(linesLabel, "LINES\n%d", lines);
    lv_obj_set_style_text_color(linesLabel, AERO_COLOR_TEXT, 0);
    lv_obj_set_style_text_align(linesLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(linesLabel, LV_ALIGN_TOP_MID, 0, 250);
    
    // Create control buttons
    pauseButton = lv_btn_create(infoPanel);
    lv_obj_set_size(pauseButton, 140, 50);
    lv_obj_align(pauseButton, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_set_style_bg_color(pauseButton, AERO_COLOR_BUTTON, 0);
    lv_obj_add_event_cb(pauseButton, [](lv_event_t* e) {
        AeroBlocks* game = static_cast<AeroBlocks*>(lv_event_get_user_data(e));
        if (game->gameState == AERO_STATE_PLAYING) {
            game->pause();
            lv_label_set_text(lv_obj_get_child(game->pauseButton, 0), "Resume");
        } else if (game->gameState == AERO_STATE_PAUSED) {
            game->resume();
            lv_label_set_text(lv_obj_get_child(game->pauseButton, 0), "Pause");
        }
    }, LV_EVENT_CLICKED, this);
    
    lv_obj_t* pauseLabel = lv_label_create(pauseButton);
    lv_label_set_text(pauseLabel, "Pause");
    lv_obj_center(pauseLabel);
    
    // Create touch control buttons
    // Left button
    leftButton = lv_btn_create(gameScreen);
    lv_obj_set_size(leftButton, 60, 60);
    lv_obj_set_pos(leftButton, boardX - 80, boardY + boardHeight - 100);
    lv_obj_set_style_bg_color(leftButton, AERO_COLOR_BUTTON, 0);
    lv_obj_set_style_radius(leftButton, 30, 0);
    lv_obj_add_event_cb(leftButton, [](lv_event_t* e) {
        AeroBlocks* game = static_cast<AeroBlocks*>(lv_event_get_user_data(e));
        if (game->gameState == AERO_STATE_PLAYING) {
            game->moveLeft();
        }
    }, LV_EVENT_CLICKED, this);
    
    lv_obj_t* leftIcon = lv_label_create(leftButton);
    lv_label_set_text(leftIcon, LV_SYMBOL_LEFT);
    lv_obj_center(leftIcon);
    
    // Right button
    rightButton = lv_btn_create(gameScreen);
    lv_obj_set_size(rightButton, 60, 60);
    lv_obj_set_pos(rightButton, boardX + boardWidth + 20, boardY + boardHeight - 100);
    lv_obj_set_style_bg_color(rightButton, AERO_COLOR_BUTTON, 0);
    lv_obj_set_style_radius(rightButton, 30, 0);
    lv_obj_add_event_cb(rightButton, [](lv_event_t* e) {
        AeroBlocks* game = static_cast<AeroBlocks*>(lv_event_get_user_data(e));
        if (game->gameState == AERO_STATE_PLAYING) {
            game->moveRight();
        }
    }, LV_EVENT_CLICKED, this);
    
    lv_obj_t* rightIcon = lv_label_create(rightButton);
    lv_label_set_text(rightIcon, LV_SYMBOL_RIGHT);
    lv_obj_center(rightIcon);
    
    // Rotate button
    rotateButton = lv_btn_create(gameScreen);
    lv_obj_set_size(rotateButton, 60, 60);
    lv_obj_set_pos(rotateButton, boardX + boardWidth / 2 - 30, boardY - 80);
    lv_obj_set_style_bg_color(rotateButton, AERO_COLOR_BUTTON, 0);
    lv_obj_set_style_radius(rotateButton, 30, 0);
    lv_obj_add_event_cb(rotateButton, [](lv_event_t* e) {
        AeroBlocks* game = static_cast<AeroBlocks*>(lv_event_get_user_data(e));
        if (game->gameState == AERO_STATE_PLAYING) {
            game->rotate();
        }
    }, LV_EVENT_CLICKED, this);
    
    lv_obj_t* rotateIcon = lv_label_create(rotateButton);
    lv_label_set_text(rotateIcon, LV_SYMBOL_LOOP);
    lv_obj_center(rotateIcon);
    
    // Drop button
    dropButton = lv_btn_create(gameScreen);
    lv_obj_set_size(dropButton, 60, 60);
    lv_obj_set_pos(dropButton, boardX + boardWidth / 2 - 30, boardY + boardHeight + 20);
    lv_obj_set_style_bg_color(dropButton, AERO_COLOR_BUTTON, 0);
    lv_obj_set_style_radius(dropButton, 30, 0);
    lv_obj_add_event_cb(dropButton, [](lv_event_t* e) {
        AeroBlocks* game = static_cast<AeroBlocks*>(lv_event_get_user_data(e));
        if (game->gameState == AERO_STATE_PLAYING) {
            game->hardDrop();
        }
    }, LV_EVENT_CLICKED, this);
    
    lv_obj_t* dropIcon = lv_label_create(dropButton);
    lv_label_set_text(dropIcon, LV_SYMBOL_DOWN);
    lv_obj_center(dropIcon);
    
    // Exit button
    lv_obj_t* exitButton = lv_btn_create(gameScreen);
    lv_obj_set_size(exitButton, 60, 40);
    lv_obj_set_pos(exitButton, 10, 10);
    lv_obj_set_style_bg_color(exitButton, AERO_COLOR_BUTTON, 0);
    lv_obj_add_event_cb(exitButton, [](lv_event_t* e) {
        AeroBlocks* game = static_cast<AeroBlocks*>(lv_event_get_user_data(e));
        game->stop();
    }, LV_EVENT_CLICKED, this);
    
    lv_obj_t* exitLabel = lv_label_create(exitButton);
    lv_label_set_text(exitLabel, LV_SYMBOL_CLOSE);
    lv_obj_center(exitLabel);
    
    // Initialize board grid
    for (int y = 0; y < AERO_BOARD_HEIGHT; y++) {
        for (int x = 0; x < AERO_BOARD_WIDTH; x++) {
            // Create grid cell object
            boardObjects[y][x] = lv_obj_create(gameBoard);
            lv_obj_set_size(boardObjects[y][x], AERO_GRID_SIZE - 1, AERO_GRID_SIZE - 1);
            lv_obj_set_pos(boardObjects[y][x], x * AERO_GRID_SIZE, y * AERO_GRID_SIZE);
            
            // Set empty cell style
            lv_obj_set_style_bg_color(boardObjects[y][x], lv_color_darken(AERO_COLOR_BACKGROUND, 10), 0);
            lv_obj_set_style_border_width(boardObjects[y][x], 1, 0);
            lv_obj_set_style_border_color(boardObjects[y][x], AERO_COLOR_GRID, 0);
            lv_obj_set_style_radius(boardObjects[y][x], 0, 0);
        }
    }
    
    // Add cloud animations
    addCloudAnimation();
    
    // Draw the initial game state
    drawBoard();
    updatePiecePreview();
    updateScoreDisplay();
    
    // Set up game timer for piece movement and updates
    lv_timer_t* game_timer = lv_timer_create(updateCallback, 50, this);
    
    // Initialize lastDropTime
    lastDropTime = millis();
    
    // Load game screen
    lv_scr_load(gameScreen);
    
    if(DEBUG) Serial.println("Game screen created");
}

// Game update callback - called by timer
void AeroBlocks::updateCallback(lv_timer_t* timer) {
    AeroBlocks* game = static_cast<AeroBlocks*>(timer->user_data);
    
    // Only process if game is in playing state
    if (game->gameState != AERO_STATE_PLAYING) {
        return;
    }
    
    // Check if it's time to drop the piece
    uint32_t currentTime = millis();
    if (currentTime - game->lastDropTime >= game->dropInterval) {
        game->lastDropTime = currentTime;
        game->moveDown();
    }
    
    // Reset watchdog
    esp_task_wdt_reset();
}

// Draw current game board state
void AeroBlocks::drawBoard() {
    // Draw fixed pieces on the board
    for (int y = 0; y < AERO_BOARD_HEIGHT; y++) {
        for (int x = 0; x < AERO_BOARD_WIDTH; x++) {
            uint8_t cellValue = board[y][x];
            
            if (cellValue > 0) {
                // Cell has a piece, set the appropriate color
                lv_obj_set_style_bg_color(boardObjects[y][x], getPieceColor(cellValue - 1), 0);
                lv_obj_set_style_bg_opa(boardObjects[y][x], LV_OPA_COVER, 0);
            } else {
                // Empty cell, set to background color
                lv_obj_set_style_bg_color(boardObjects[y][x], lv_color_darken(AERO_COLOR_BACKGROUND, 10), 0);
                lv_obj_set_style_bg_opa(boardObjects[y][x], LV_OPA_COVER, 0);
            }
        }
    }
    
    // Draw current falling piece
    if (gameState == AERO_STATE_PLAYING) {
        drawPiece(currentPieceType, currentRotation, currentX, currentY, gameBoard);
    }
}

// Draw a tetromino piece at specified position
void AeroBlocks::drawPiece(int pieceType, int rotation, int x, int y, lv_obj_t* parent, bool preview) {
    uint16_t piecePattern = pieceDefinitions[pieceType][rotation];
    lv_color_t color = getPieceColor(pieceType);
    
    // Iterate through the 4x4 piece grid
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            // Check if this cell of the piece is filled
            if ((piecePattern & (1 << (row * 4 + col))) != 0) {
                // Calculate position on the board
                int boardX = x + col;
                int boardY = y + row;
                
                // Check if this position is valid
                if (boardX >= 0 && boardX < AERO_BOARD_WIDTH && 
                    boardY >= 0 && boardY < AERO_BOARD_HEIGHT && !preview) {
                    // Update the cell on the game board
                    lv_obj_set_style_bg_color(boardObjects[boardY][boardX], color, 0);
                } else if (preview) {
                    // Draw on the preview panel
                    int previewSize = AERO_GRID_SIZE * 0.8; // Slightly smaller for preview
                    int offsetX = (AERO_NEXT_PIECE_SIZE * AERO_GRID_SIZE - 4 * previewSize) / 2;
                    int offsetY = (AERO_NEXT_PIECE_SIZE * AERO_GRID_SIZE - 4 * previewSize) / 2;
                    
                    // Create a preview block
                    lv_obj_t* block = lv_obj_create(parent);
                    lv_obj_set_size(block, previewSize - 1, previewSize - 1);
                    lv_obj_set_pos(block, offsetX + col * previewSize, offsetY + row * previewSize);
                    lv_obj_set_style_bg_color(block, color, 0);
                    lv_obj_set_style_border_width(block, 1, 0);
                    lv_obj_set_style_border_color(block, lv_color_darken(color, 20), 0);
                }
            }
        }
    }
}

// Clear the next piece preview panel
void AeroBlocks::clearPreview() {
    // Delete all children of the preview panel
    lv_obj_clean(nextPiecePreview);
}

// Update the next piece preview display
void AeroBlocks::updatePiecePreview() {
    // Clear existing preview
    clearPreview();
    
    // Draw the next piece
    drawPiece(nextPieceType, 0, 0, 0, nextPiecePreview, true);
}

// Update score, level, and lines display
void AeroBlocks::updateScoreDisplay() {
    lv_label_set_text_fmt(scoreLabel, "SCORE\n%d", score);
    lv_label_set_text_fmt(levelLabel, "LEVEL\n%d", level);
    lv_label_set_text_fmt(linesLabel, "LINES\n%d", lines);
}

// Get color for a specific piece type
lv_color_t AeroBlocks::getPieceColor(int pieceType) {
    switch (pieceType) {
        case AERO_FIGHTER:     return AERO_COLOR_FIGHTER;
        case AERO_BOMBER:      return AERO_COLOR_BOMBER;
        case AERO_STEALTH:     return AERO_COLOR_STEALTH;
        case AERO_CARGO:       return AERO_COLOR_CARGO;
        case AERO_HELICOPTER:  return AERO_COLOR_HELICOPTER;
        case AERO_SEAPLANE:    return AERO_COLOR_SEAPLANE;
        case AERO_BIPLANE:     return AERO_COLOR_BIPLANE;
        default:               return lv_color_hex(0xFFFFFF); // White as fallback
    }
}

// Add decorative cloud animations in the background
void AeroBlocks::addCloudAnimation() {
    // Create 5 cloud objects that slowly move across the screen
    for (int i = 0; i < 5; i++) {
        lv_obj_t* cloud = lv_obj_create(gameScreen);
        int cloudWidth = 60 + (i * 20);
        int cloudHeight = 20 + (i % 3) * 10;
        lv_obj_set_size(cloud, cloudWidth, cloudHeight);
        lv_obj_set_style_radius(cloud, cloudHeight / 2, 0);
        lv_obj_set_style_bg_color(cloud, AERO_COLOR_CLOUDS, 0);
        lv_obj_set_style_bg_opa(cloud, 80, 0); // Semi-transparent
        lv_obj_set_style_border_width(cloud, 0, 0);
        
        // Set random initial positions
        int startX = -cloudWidth + (i * 50);
        int y = 50 + (i * 80) % (SCREEN_HEIGHT - 100);
        lv_obj_set_pos(cloud, startX, y);
        
        // Create animation to move clouds slowly across the screen
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, cloud);
        lv_anim_set_exec_cb(&a, [](void* var, int32_t v) {
            lv_obj_set_x((lv_obj_t*)var, v);
        });
        lv_anim_set_values(&a, startX, SCREEN_WIDTH + 50);
        lv_anim_set_time(&a, 30000 + i * 5000); // 30-50 seconds to cross
        lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
        lv_anim_set_repeat_delay(&a, 2000 + i * 1000);
        lv_anim_start(&a);
    }
}
