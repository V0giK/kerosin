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
#include <stdlib.h> // Für rand()
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "uartCommunication.h"  // Hinzufügen der UartCommunication-Header-Datei
#include "helper.h"  // Hinzufügen des helper.h Headers für COM_ID_STATUS

AeroBlocks::AeroBlocks() : 
    gameState(AERO_STATE_TITLE),
    gameScreen(nullptr),
    titleScreen(nullptr),
    gameOverScreen(nullptr),
    highScoreScreen(nullptr),
    previousScreen(nullptr),
    gameBoard(nullptr),  // Korrigiert: Komma durch schließende Klammer und Komma ersetzt
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
    pauseButton(nullptr),
    heartbeatTimer(nullptr),
    uartCom(nullptr)  // Initialisierung des UART-Kommunikationszeigers
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
    
    // Load high scores from persistent storage
    loadHighScores();
    
    // Heartbeat-Timer starten
    if (heartbeatTimer != nullptr) {
        lv_timer_del(heartbeatTimer);
    }
    heartbeatTimer = lv_timer_create(heartbeatCallback, 3000, this); // Alle 3 Sekunden
    
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
    
    // Heartbeat-Timer stoppen
    if (heartbeatTimer) {
        lv_timer_del(heartbeatTimer);
        heartbeatTimer = nullptr;
    }
    
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
    lv_obj_t* btn = (lv_obj_t*)lv_event_get_current_target(e);
    int userData = (int)(uintptr_t)lv_obj_get_user_data(btn);

    if(DEBUG) Serial.printf("Button event: %d\n", userData);

    switch (userData) {
        case 1: // Start Game button
            if(DEBUG) Serial.println("Start Game button pressed");
            // Prüfe, ob titleScreen existiert, bevor gelöscht wird
            if (game->titleScreen) {
                lv_obj_del(game->titleScreen);
                game->titleScreen = nullptr;
            }
            // Setze alle relevanten Pointer auf nullptr, um Double-Free zu vermeiden
            game->gameScreen = nullptr;
            game->gameBoard = nullptr;
            game->scoreLabel = nullptr;
            game->levelLabel = nullptr;
            game->linesLabel = nullptr;
            game->nextPiecePreview = nullptr;
            game->leftButton = nullptr;
            game->rightButton = nullptr;
            game->rotateButton = nullptr;
            game->dropButton = nullptr;
            game->pauseButton = nullptr;
            // Reset und dann GameScreen erzeugen
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

// Replace the placeholder high score screen with the full implementation
void AeroBlocks::createHighScoreScreen() {
    if(DEBUG) Serial.println("Creating high score screen");
    
    // Create high score screen
    highScoreScreen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(highScoreScreen, AERO_COLOR_BACKGROUND, 0);
    
    // Create title
    lv_obj_t* titleLabel = lv_label_create(highScoreScreen);
    lv_label_set_text(titleLabel, "HIGH SCORES");
    lv_obj_set_style_text_color(titleLabel, AERO_COLOR_TEXT, 0);
    lv_obj_set_style_text_font(titleLabel, &lv_font_montserrat_30, 0);
    lv_obj_align(titleLabel, LV_ALIGN_TOP_MID, 0, 20);
    
    // Finde den längsten Pilotennamen
    size_t maxNameLength = 0;
    for (const auto& score : highScores) {
        maxNameLength = std::max(maxNameLength, score.getName().length());
    }
    // Minimum von 9 Zeichen (für "PILOT" Überschrift)
    maxNameLength = std::max(maxNameLength, (size_t)9);
    
    // Berechne Spaltenbreiten basierend auf dem längsten Namen
    // Pro Zeichen ca. 8 Pixel Breite + 10 Pixel Puffer
    int pilotColumnWidth = maxNameLength * 8 + 10;
    
    // Linker Rand bleibt bei 10px
    int leftMargin = 10;
    
    // Berechne neue Panel-Breite (erweitert) und Spalten-Positionen
    // Grundbreite um 30px erhöht (380 statt 370) für mehr Rand-Abstand
    int panelWidth = 380 + (pilotColumnWidth > 80 ? pilotColumnWidth - 80 : 0);
    
    // Definiere angepasste Spaltenbreiten und Positionen
    const int colX[] = {leftMargin, leftMargin + 60, leftMargin + 60 + pilotColumnWidth, 
                         leftMargin + 60 + pilotColumnWidth + 70, leftMargin + 60 + pilotColumnWidth + 140};
    
    // Create a panel for the scores - breiter machen, basierend auf dem längsten Namen
    lv_obj_t* scorePanel = lv_obj_create(highScoreScreen);
    lv_obj_set_size(scorePanel, panelWidth, 320);
    lv_obj_align(scorePanel, LV_ALIGN_TOP_MID, 0, 80);
    lv_obj_set_style_bg_color(scorePanel, lv_color_darken(AERO_COLOR_BACKGROUND, 20), 0);
    lv_obj_set_style_border_width(scorePanel, 2, 0);
    lv_obj_set_style_border_color(scorePanel, AERO_COLOR_GRID, 0);
    lv_obj_set_style_radius(scorePanel, 10, 0);
    lv_obj_clear_flag(scorePanel, LV_OBJ_FLAG_SCROLLABLE); // Scrollbalken deaktivieren
    
    // Erstelle Spaltenüberschriften
    const char* headers[] = {"RANK", "PILOT", "SCORE", "LVL", "LINES"};
    for (int i = 0; i < 5; i++) {
        lv_obj_t* headerLabel = lv_label_create(scorePanel);
        lv_label_set_text(headerLabel, headers[i]);
        lv_obj_set_style_text_color(headerLabel, AERO_COLOR_CLOUDS, 0);
        lv_obj_set_pos(headerLabel, colX[i], 10);
    }
    
    // Display each high score with separate labels for each column
    for (size_t i = 0; i < highScores.size(); i++) {
        const auto& score = highScores[i];
        int y = 40 + i * 30;
        
        // Rank
        lv_obj_t* rankLabel = lv_label_create(scorePanel);
        char rankText[8];
        snprintf(rankText, sizeof(rankText), "%d", (int)(i + 1));
        lv_label_set_text(rankLabel, rankText);
        lv_obj_set_style_text_color(rankLabel, AERO_COLOR_TEXT, 0);
        lv_obj_set_pos(rankLabel, colX[0], y);
        
        // Pilot name
        lv_obj_t* nameLabel = lv_label_create(scorePanel);
        lv_label_set_text(nameLabel, score.getName().c_str());
        lv_obj_set_style_text_color(nameLabel, AERO_COLOR_TEXT, 0);
        lv_obj_set_pos(nameLabel, colX[1], y);
        
        // Score
        lv_obj_t* scoreValueLabel = lv_label_create(scorePanel);
        char scoreText[16];
        snprintf(scoreText, sizeof(scoreText), "%d", score.getScore());
        lv_label_set_text(scoreValueLabel, scoreText);
        lv_obj_set_style_text_color(scoreValueLabel, AERO_COLOR_TEXT, 0);
        lv_obj_set_pos(scoreValueLabel, colX[2], y);
        
        // Level
        lv_obj_t* levelLabel = lv_label_create(scorePanel);
        char levelText[8];
        snprintf(levelText, sizeof(levelText), "%d", score.getLevel());
        lv_label_set_text(levelLabel, levelText);
        lv_obj_set_style_text_color(levelLabel, AERO_COLOR_TEXT, 0);
        lv_obj_set_pos(levelLabel, colX[3], y);
        
        // Lines
        lv_obj_t* linesLabel = lv_label_create(scorePanel);
        char linesText[8];
        snprintf(linesText, sizeof(linesText), "%d", score.getLines());
        lv_label_set_text(linesLabel, linesText);
        lv_obj_set_style_text_color(linesLabel, AERO_COLOR_TEXT, 0);
        lv_obj_set_pos(linesLabel, colX[4], y);
    }
    
    // Create back button
    lv_obj_t* backBtn = lv_btn_create(highScoreScreen);
    lv_obj_set_size(backBtn, 120, 50);
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
    
    // Add decorative airplane symbols
    addHighScoreDecoration(highScoreScreen);
    
    // Load high score screen
    lv_scr_load(highScoreScreen);
}

// Create name entry screen for high score
void AeroBlocks::createNameEntryScreen() {
    if(DEBUG) Serial.println("Creating name entry screen");
    
    // Create new screen
    nameEntryScreen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(nameEntryScreen, AERO_COLOR_BACKGROUND, 0);
    
    // Create title
    lv_obj_t* titleLabel = lv_label_create(nameEntryScreen);
    lv_label_set_text(titleLabel, "NEW HIGH SCORE!");
    lv_obj_set_style_text_color(titleLabel, AERO_COLOR_TEXT, 0);
    lv_obj_set_style_text_font(titleLabel, &lv_font_montserrat_30, 0);
    lv_obj_align(titleLabel, LV_ALIGN_TOP_MID, 0, 20);
    
    // Display score
    lv_obj_t* scoreLabel = lv_label_create(nameEntryScreen);
    lv_label_set_text_fmt(scoreLabel, "Score: %d\nLevel: %d\nLines: %d", score, level, lines);
    lv_obj_set_style_text_color(scoreLabel, AERO_COLOR_TEXT, 0);
    lv_obj_align(scoreLabel, LV_ALIGN_TOP_MID, 0, 70);
    
    // Create name entry field
    lv_obj_t* nameLabel = lv_label_create(nameEntryScreen);
    lv_label_set_text(nameLabel, "Enter your name:");
    lv_obj_set_style_text_color(nameLabel, AERO_COLOR_TEXT, 0);
    lv_obj_align(nameLabel, LV_ALIGN_TOP_MID, 0, 130);
    
    // Create text area for name input
    nameTextfield = lv_textarea_create(nameEntryScreen);
    lv_obj_set_size(nameTextfield, 200, 50);
    lv_textarea_set_max_length(nameTextfield, 9);
    lv_textarea_set_one_line(nameTextfield, true);
    lv_textarea_set_text(nameTextfield, "PILOT");
    lv_obj_align(nameTextfield, LV_ALIGN_TOP_MID, 0, 160);
    lv_obj_add_state(nameTextfield, LV_STATE_FOCUSED);
    
    // Create keyboard
    lv_obj_t* kb = lv_keyboard_create(nameEntryScreen);
    lv_keyboard_set_textarea(kb, nameTextfield);
    lv_obj_align(kb, LV_ALIGN_BOTTOM_MID, 0, 0);
    
    // Create submit button
    lv_obj_t* submitBtn = lv_btn_create(nameEntryScreen);
    lv_obj_set_size(submitBtn, 120, 50);
    lv_obj_set_style_bg_color(submitBtn, AERO_COLOR_BUTTON, 0);
    lv_obj_align(submitBtn, LV_ALIGN_TOP_MID, 0, 230);
    lv_obj_add_event_cb(submitBtn, [](lv_event_t* e) {
        AeroBlocks* game = static_cast<AeroBlocks*>(lv_event_get_user_data(e));
        
        // Get the player's name from text field
        const char* name = lv_textarea_get_text(game->nameTextfield);
        
        // Add to high scores
        game->addHighScore(game->score, game->level, game->lines, name);
        
        // Clean up name entry screen
        if (game->nameEntryScreen) {
            lv_obj_del(game->nameEntryScreen);
            game->nameEntryScreen = nullptr;
            game->nameTextfield = nullptr;
        }
        
        // Show high scores
        game->createHighScoreScreen();
    }, LV_EVENT_CLICKED, this);
    
    lv_obj_t* submitLabel = lv_label_create(submitBtn);
    lv_label_set_text(submitLabel, "Submit");
    lv_obj_center(submitLabel);
    
    // Load name entry screen
    lv_scr_load(nameEntryScreen);
}

// Implement proper high score loading from file system
void AeroBlocks::loadHighScores() {
    if(DEBUG) Serial.println("Loading high scores from storage");
    
    // Make sure filesystem is mounted
    if (!LittleFS.begin(true)) {
        if(DEBUG) Serial.println("Failed to mount file system");
        return;
    }
    
    // Check if high score file exists
    if (!LittleFS.exists("/aero_scores.json")) {
        if(DEBUG) Serial.println("No high score file found, using defaults");
        return;
    }
    
    // Open the file for reading
    File file = LittleFS.open("/aero_scores.json", "r");
    if (!file) {
        if(DEBUG) Serial.println("Failed to open high score file for reading");
        return;
    }
    
    // Parse JSON document
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    
    if (error) {
        if(DEBUG) Serial.printf("Failed to parse high scores: %s\n", error.c_str());
        return;
    }
    
    // Clear existing high scores
    highScores.clear();
    
    // Read high scores from JSON document
    JsonArray scoresArray = doc["scores"].as<JsonArray>();
    for (JsonObject scoreObj : scoresArray) {
        int score = scoreObj["score"];
        int level = scoreObj["level"];
        int lines = scoreObj["lines"];
        const char* name = scoreObj["name"];
        
        highScores.push_back(AeroHighScore(score, level, lines, name));
    }
    
    // Sort the high scores
    std::sort(highScores.begin(), highScores.end());
    
    if(DEBUG) Serial.printf("Loaded %d high scores\n", highScores.size());
}

// Save high scores to persistent storage
void AeroBlocks::saveHighScores() {
    if(DEBUG) Serial.println("Saving high scores to storage");
    
    // Make sure filesystem is mounted
    if (!LittleFS.begin(true)) {
        if(DEBUG) Serial.println("Failed to mount file system");
        return;
    }
    
    // Create JSON document
    StaticJsonDocument<1024> doc;
    JsonArray scoresArray = doc.createNestedArray("scores");
    
    // Add high scores to JSON document
    for (const auto& score : highScores) {
        JsonObject scoreObj = scoresArray.createNestedObject();
        scoreObj["score"] = score.getScore();
        scoreObj["level"] = score.getLevel();
        scoreObj["lines"] = score.getLines();
        scoreObj["name"] = score.getName();
    }
    
    // Open the file for writing
    File file = LittleFS.open("/aero_scores.json", "w");
    if (!file) {
        if(DEBUG) Serial.println("Failed to open high score file for writing");
        return;
    }
    
    // Serialize JSON to file
    if (serializeJson(doc, file) == 0) {
        if(DEBUG) Serial.println("Failed to write high scores to file");
    }
    
    file.close();
    if(DEBUG) Serial.println("High scores saved successfully");
}

// Check if score qualifies for high scores and add it
bool AeroBlocks::addHighScore(int score, int level, int lines, const char* name) {
    if(DEBUG) Serial.printf("Checking if score %d qualifies for high scores\n", score);
    
    // If we have fewer than max scores, or score is higher than lowest score
    if (highScores.size() < AERO_HIGHSCORE_COUNT || 
        score > highScores[highScores.size() - 1].getScore()) {
        
        // Add the new score
        highScores.push_back(AeroHighScore(score, level, lines, name));
        
        // Sort high scores
        std::sort(highScores.begin(), highScores.end());
        
        // Trim to max size
        if (highScores.size() > AERO_HIGHSCORE_COUNT) {
            highScores.resize(AERO_HIGHSCORE_COUNT);
        }
        
        // Save to storage
        saveHighScores();
        return true;
    }
    
    return false;
}

// Create the game over screen - Updated with high score check
void AeroBlocks::createGameOverScreen() {
    if (DEBUG) Serial.println("Creating game over screen");

    // Lösche ggf. vorhandene GameOver-Screen
    if (gameOverScreen) {
        lv_obj_del(gameOverScreen);
        gameOverScreen = nullptr;
    }

    // Check if this score qualifies for high scores
    bool isHighScore = false;
    if (highScores.size() < AERO_HIGHSCORE_COUNT || score > highScores[highScores.size() - 1].getScore()) {
        isHighScore = true;
        // Show name entry screen instead of game over
        createNameEntryScreen();
        return;
    }

    // Continue with normal game over screen if not a high score
    gameOverScreen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(gameOverScreen, AERO_COLOR_BACKGROUND, 0);

    // Game Over Label
    lv_obj_t* overLabel = lv_label_create(gameOverScreen);
    lv_label_set_text(overLabel, "GAME OVER");
    lv_obj_set_style_text_color(overLabel, AERO_COLOR_BOMBER, 0);
    lv_obj_set_style_text_font(overLabel, &lv_font_montserrat_30, 0);
    lv_obj_align(overLabel, LV_ALIGN_TOP_MID, 0, 40);

    // Score anzeigen
    lv_obj_t* scoreLabelGO = lv_label_create(gameOverScreen);
    lv_label_set_text_fmt(scoreLabelGO, "Score: %d\nLevel: %d\nLines: %d", score, level, lines);
    lv_obj_set_style_text_color(scoreLabelGO, AERO_COLOR_TEXT, 0);
    lv_obj_set_style_text_font(scoreLabelGO, &lv_font_montserrat_18, 0);
    lv_obj_align(scoreLabelGO, LV_ALIGN_TOP_MID, 0, 100);

    // Restart Button
    lv_obj_t* restartBtn = lv_btn_create(gameOverScreen);
    lv_obj_set_size(restartBtn, 180, 60);
    lv_obj_set_style_bg_color(restartBtn, AERO_COLOR_BUTTON, 0);
    lv_obj_align(restartBtn, LV_ALIGN_BOTTOM_MID, 0, -150);
    lv_obj_add_event_cb(restartBtn, [](lv_event_t* e) {
        AeroBlocks* game = static_cast<AeroBlocks*>(lv_event_get_user_data(e));
        if (game->gameOverScreen) {
            lv_obj_del(game->gameOverScreen);
            game->gameOverScreen = nullptr;
        }
        game->resetGame();
        game->createGameScreen();
    }, LV_EVENT_CLICKED, this);

    lv_obj_t* restartLabel = lv_label_create(restartBtn);
    lv_label_set_text(restartLabel, "Restart");
    lv_obj_center(restartLabel);

    // High Scores Button
    lv_obj_t* highScoresBtn = lv_btn_create(gameOverScreen);
    lv_obj_set_size(highScoresBtn, 180, 60);
    lv_obj_set_style_bg_color(highScoresBtn, AERO_COLOR_BUTTON, 0);
    lv_obj_align(highScoresBtn, LV_ALIGN_BOTTOM_MID, 0, -80);
    lv_obj_add_event_cb(highScoresBtn, [](lv_event_t* e) {
        AeroBlocks* game = static_cast<AeroBlocks*>(lv_event_get_user_data(e));
        if (game->gameOverScreen) {
            lv_obj_del(game->gameOverScreen);
            game->gameOverScreen = nullptr;
        }
        game->createHighScoreScreen();
    }, LV_EVENT_CLICKED, this);

    lv_obj_t* highScoresLabel = lv_label_create(highScoresBtn);
    lv_label_set_text(highScoresLabel, "High Scores");
    lv_obj_center(highScoresLabel);

    // Back to Title Button
    lv_obj_t* titleBtn = lv_btn_create(gameOverScreen);
    lv_obj_set_size(titleBtn, 180, 60);
    lv_obj_set_style_bg_color(titleBtn, AERO_COLOR_BUTTON, 0);
    lv_obj_align(titleBtn, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_add_event_cb(titleBtn, [](lv_event_t* e) {
        AeroBlocks* game = static_cast<AeroBlocks*>(lv_event_get_user_data(e));
        if (game->gameOverScreen) {
            lv_obj_del(game->gameOverScreen);
            game->gameOverScreen = nullptr;
        }
        game->createTitleScreen();
    }, LV_EVENT_CLICKED, this);

    lv_obj_t* titleLabel = lv_label_create(titleBtn);
    lv_label_set_text(titleLabel, "Back to Title");
    lv_obj_center(titleLabel);

    lv_scr_load(gameOverScreen);

    if (DEBUG) Serial.println("Game over screen created");
}

// Move the function implementation closer to other UI-related functions
// and make sure it's properly scoped to the AeroBlocks class
void AeroBlocks::addHighScoreDecoration(lv_obj_t* screen) {
    // Create decorative airplane symbols in the background
    lv_obj_t* planeIcon1 = lv_obj_create(screen);
    lv_obj_set_size(planeIcon1, 120, 20);
    lv_obj_set_style_radius(planeIcon1, 10, 0);
    lv_obj_set_style_bg_color(planeIcon1, AERO_COLOR_FIGHTER, 0);
    lv_obj_align(planeIcon1, LV_ALIGN_LEFT_MID, 50, -120);
    lv_obj_set_style_transform_angle(planeIcon1, 330, 0);
    
    lv_obj_t* planeIcon2 = lv_obj_create(screen);
    lv_obj_set_size(planeIcon2, 120, 20);
    lv_obj_set_style_radius(planeIcon2, 10, 0);
    lv_obj_set_style_bg_color(planeIcon2, AERO_COLOR_BOMBER, 0);
    lv_obj_align(planeIcon2, LV_ALIGN_RIGHT_MID, -50, 120);
    lv_obj_set_style_transform_angle(planeIcon2, 30, 0);
}

// Reset game to initial state
void AeroBlocks::resetGame() {
    if(DEBUG) Serial.println("Resetting game state");
    
    // Reset board
    memset(board, 0, sizeof(board));
    
    // Reset score and game variables
    score = 0;
    level = 1;
    lines = 0;
    linesForNextLevel = 10;
    dropInterval = 1000; // 1 second at start
    
    // Generate initial pieces
    currentPieceType = rand() % AERO_PIECE_COUNT;
    nextPieceType = rand() % AERO_PIECE_COUNT;
    currentRotation = 0;
    currentX = (AERO_BOARD_WIDTH / 2) - 2; // Center the piece
    currentY = -2; // Start just above the visible board
    
    // Set game state to playing
    gameState = AERO_STATE_PLAYING;
    
    // Record game start time
    gameStartTime = millis();
    
    if(DEBUG) Serial.println("Game reset complete");
}

// Create proper game screen with board, controls and UI elements
void AeroBlocks::createGameScreen() {
    if(DEBUG) Serial.println("Creating game screen");

    // Create main game screen
    gameScreen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(gameScreen, AERO_COLOR_BACKGROUND, 0);
    lv_obj_clear_flag(gameScreen, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling

    // Calculate board dimensions and position
    int boardWidth = AERO_GRID_SIZE * AERO_BOARD_WIDTH + 2;
    int boardHeight = AERO_GRID_SIZE * AERO_BOARD_HEIGHT + 2;
    int infoPanelWidth = 180;
    int controlsHeight = 60 + 20; // Button height + spacing
    int totalHeight = boardHeight > controlsHeight ? boardHeight : controlsHeight;
    int screenContentHeight = totalHeight;
    if (boardHeight + controlsHeight + 40 > SCREEN_HEIGHT) {
        // Buttons and game board don't fit, move buttons closer to board
        controlsHeight = SCREEN_HEIGHT - boardHeight - 40;
        if (controlsHeight < 60) controlsHeight = 60; // Minimum height
        screenContentHeight = boardHeight + controlsHeight;
    }

    int boardX = (SCREEN_WIDTH - boardWidth - infoPanelWidth - 20) / 2;
    int boardY = (SCREEN_HEIGHT - screenContentHeight) / 2;

    // Create the game board container (frame)
    gameBoard = lv_obj_create(gameScreen);
    lv_obj_set_size(gameBoard, boardWidth, boardHeight);
    lv_obj_set_pos(gameBoard, boardX, boardY);
    lv_obj_set_style_bg_color(gameBoard, AERO_COLOR_BACKGROUND, 0);
    lv_obj_set_style_border_color(gameBoard, AERO_COLOR_GRID, 0);
    lv_obj_set_style_border_width(gameBoard, 2, 0);
    lv_obj_set_style_pad_all(gameBoard, 0, 0);
    lv_obj_clear_flag(gameBoard, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling

    // Create info panel
    lv_obj_t* infoPanel = lv_obj_create(gameScreen);
    lv_obj_set_size(infoPanel, 180, boardHeight);
    lv_obj_set_pos(infoPanel, boardX + boardWidth + 20, boardY);
    lv_obj_set_style_bg_color(infoPanel, lv_color_darken(AERO_COLOR_BACKGROUND, 20), 0);
    lv_obj_set_style_radius(infoPanel, 10, 0);
    lv_obj_set_style_border_width(infoPanel, 2, 0);
    lv_obj_set_style_border_color(infoPanel, AERO_COLOR_GRID, 0);
    lv_obj_clear_flag(infoPanel, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling

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
    lv_obj_clear_flag(nextPiecePreview, LV_OBJ_FLAG_SCROLLABLE); // Remove scrollbar

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
    lv_obj_align(pauseButton, LV_ALIGN_BOTTOM_MID, 0, 0);
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
    
    // Position the control buttons below the game board, not above!
    int controlsY = boardY + boardHeight + 10;
    if (controlsY + 60 > SCREEN_HEIGHT) {
        controlsY = SCREEN_HEIGHT - 60 - 10; // 60=button height, 10=spacing
    }
    int controlsCenterX = boardX + (boardWidth / 2);

    // Left button
    leftButton = lv_btn_create(gameScreen);
    lv_obj_set_size(leftButton, 60, 60);
    lv_obj_set_pos(leftButton, controlsCenterX - 100, controlsY);
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

    // Drop button (center bottom)
    dropButton = lv_btn_create(gameScreen);
    lv_obj_set_size(dropButton, 60, 60);
    lv_obj_set_pos(dropButton, controlsCenterX - 30, controlsY);
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

    // Right button
    rightButton = lv_btn_create(gameScreen);
    lv_obj_set_size(rightButton, 60, 60);
    lv_obj_set_pos(rightButton, controlsCenterX + 40, controlsY);
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

    // Rotate button (next to the others)
    rotateButton = lv_btn_create(gameScreen);
    lv_obj_set_size(rotateButton, 60, 60);
    lv_obj_set_pos(rotateButton, controlsCenterX + 110, controlsY);
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
    
    // Initialize board grid (place cells exactly within the frame)
    for (int y = 0; y < AERO_BOARD_HEIGHT; y++) {
        for (int x = 0; x < AERO_BOARD_WIDTH; x++) {
            boardObjects[y][x] = lv_obj_create(gameBoard);
            lv_obj_set_size(boardObjects[y][x], AERO_GRID_SIZE - 2, AERO_GRID_SIZE - 2); // -2 for spacing
            lv_obj_set_pos(boardObjects[y][x], 1 + x * AERO_GRID_SIZE, 1 + y * AERO_GRID_SIZE); // +1 for frame
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

// Pause the game
void AeroBlocks::pause() {
    if(DEBUG) Serial.println("Game paused");
    
    // Set game state to paused
    gameState = AERO_STATE_PAUSED;
    
    // Disable control buttons
    lv_obj_add_state(leftButton, LV_STATE_DISABLED);
    lv_obj_add_state(rightButton, LV_STATE_DISABLED);
    lv_obj_add_state(rotateButton, LV_STATE_DISABLED);
    lv_obj_add_state(dropButton, LV_STATE_DISABLED);
    
    // Show pause overlay with menu options
    lv_obj_t* pauseMsg = lv_obj_create(gameScreen);
    lv_obj_set_size(pauseMsg, 200, 220); // Höher für zusätzliche Buttons
    lv_obj_set_style_bg_color(pauseMsg, lv_color_darken(AERO_COLOR_BACKGROUND, 30), 0);
    lv_obj_set_style_border_width(pauseMsg, 2, 0);
    lv_obj_set_style_border_color(pauseMsg, AERO_COLOR_GRID, 0);
    lv_obj_set_style_radius(pauseMsg, 10, 0);
    lv_obj_align(pauseMsg, LV_ALIGN_CENTER, 0, 0);
    
    // Pause title - reduzierter Abstand nach oben (von 30 auf 20)
    lv_obj_t* pauseText = lv_label_create(pauseMsg);
    lv_label_set_text(pauseText, "GAME PAUSED");
    lv_obj_set_style_text_color(pauseText, AERO_COLOR_TEXT, 0);
    lv_obj_align(pauseText, LV_ALIGN_TOP_MID, 0, 20);
    
    // Resume button - reduzierter Abstand zum Titel (von 80 auf 55)
    lv_obj_t* resumeBtn = lv_btn_create(pauseMsg);
    lv_obj_set_size(resumeBtn, 160, 50);
    lv_obj_set_style_bg_color(resumeBtn, AERO_COLOR_BUTTON, 0);
    lv_obj_align(resumeBtn, LV_ALIGN_TOP_MID, 0, 55);
    lv_obj_add_event_cb(resumeBtn, [](lv_event_t* e) {
        AeroBlocks* game = static_cast<AeroBlocks*>(lv_event_get_user_data(e));
        game->resume();
    }, LV_EVENT_CLICKED, this);
    
    lv_obj_t* resumeLabel = lv_label_create(resumeBtn);
    lv_label_set_text(resumeLabel, "Resume");
    lv_obj_center(resumeLabel);
    
    // Exit button - reduzierter Abstand zum Resume-Button (von 150 auf 115)
    lv_obj_t* exitBtn = lv_btn_create(pauseMsg);
    lv_obj_set_size(exitBtn, 160, 50);
    lv_obj_set_style_bg_color(exitBtn, AERO_COLOR_BOMBER, 0); // Rote Farbe für Exit
    lv_obj_align(exitBtn, LV_ALIGN_TOP_MID, 0, 115);
    lv_obj_add_event_cb(exitBtn, [](lv_event_t* e) {
        AeroBlocks* game = static_cast<AeroBlocks*>(lv_event_get_user_data(e));
        
        // Lösche den Game-Screen
        if (game->gameScreen) {
            lv_obj_del(game->gameScreen);
            game->gameScreen = nullptr;
            game->gameBoard = nullptr;
        }
        
        // Zurück zum Titelbildschirm
        game->gameState = AERO_STATE_TITLE;
        game->createTitleScreen();
    }, LV_EVENT_CLICKED, this);
    
    lv_obj_t* exitLabel = lv_label_create(exitBtn);
    lv_label_set_text(exitLabel, "Exit to Title");
    lv_obj_center(exitLabel);
}

// Resume the paused game
void AeroBlocks::resume() {
    if(DEBUG) Serial.println("Game resumed");
    
    // Set game state back to playing
    gameState = AERO_STATE_PLAYING;
    
    // Re-enable control buttons
    lv_obj_clear_state(leftButton, LV_STATE_DISABLED);
    lv_obj_clear_state(rightButton, LV_STATE_DISABLED);
    lv_obj_clear_state(rotateButton, LV_STATE_DISABLED);
    lv_obj_clear_state(dropButton, LV_STATE_DISABLED);
    
    // Remove pause overlay
    lv_obj_t* pauseMsg = lv_obj_get_child(gameScreen, lv_obj_get_child_cnt(gameScreen) - 1);
    if (pauseMsg) {
        lv_obj_del(pauseMsg);
    }
    
    // Reset the last drop time to prevent immediate drop
    lastDropTime = millis();
}

// Move the current piece left
void AeroBlocks::moveLeft() {
    if(DEBUG) Serial.println("Moving piece left");
    
    // Check if the move is valid (no collision)
    if (!checkCollision(currentPieceType, currentRotation, currentX - 1, currentY)) {
        currentX--;
        drawBoard(); // Redraw the board with the updated piece position
    }
}

// Move the current piece right
void AeroBlocks::moveRight() {
    if(DEBUG) Serial.println("Moving piece right");
    
    // Check if the move is valid (no collision)
    if (!checkCollision(currentPieceType, currentRotation, currentX + 1, currentY)) {
        currentX++;
        drawBoard(); // Redraw the board with the updated piece position
    }
}

// Rotate the current piece
void AeroBlocks::rotate() {
    if(DEBUG) Serial.println("Rotating piece counter-clockwise");
    
    // Calculate the new rotation (0-3) in counter-clockwise direction
    // (currentRotation + 3) % 4 is equivalent to (currentRotation - 1 + 4) % 4
    int newRotation = (currentRotation + 3) % 4;
    
    // Check if the rotation is valid (no collision)
    if (!checkCollision(currentPieceType, newRotation, currentX, currentY)) {
        currentRotation = newRotation;
        drawBoard(); // Redraw the board with the updated piece rotation
    }
}

// Instantly drop the piece to the lowest possible position
void AeroBlocks::hardDrop() {
    if(DEBUG) Serial.println("Hard dropping piece");
    
    // Move the piece down as far as possible
    while (!checkCollision(currentPieceType, currentRotation, currentX, currentY + 1)) {
        currentY++;
    }
    
    // Process the landed piece
    moveDown(); // This will place the piece and spawn a new one
}

// Update callback for the game timer
void AeroBlocks::updateCallback(lv_timer_t* timer) {
    // Get the game instance from the timer's user data
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
}

// Add cloud animations to the game screen
void AeroBlocks::addCloudAnimation() {
    if(DEBUG) Serial.println("Adding cloud animations");
    
    // Create several clouds in the background
    for (int i = 0; i < 3; i++) {
        lv_obj_t* cloud = lv_obj_create(gameScreen);
        int cloudWidth = 60 + (i * 20);
        int cloudHeight = 20 + (i * 5);
        
        lv_obj_set_size(cloud, cloudWidth, cloudHeight);
        lv_obj_set_style_radius(cloud, cloudHeight / 2, 0);
        lv_obj_set_style_bg_color(cloud, AERO_COLOR_CLOUDS, 0);
        lv_obj_set_style_bg_opa(cloud, 100, 0); // Semi-transparent
        lv_obj_set_style_border_width(cloud, 0, 0);
        
        // Position the clouds
        int y = 30 + (i * 100);
        lv_obj_set_pos(cloud, -cloudWidth, y); // Start from left side
        
        // We would normally animate the clouds here, but we'll keep it simple
        // for now to avoid adding more complexity
    }
}

// Draw the current state of the game board
void AeroBlocks::drawBoard() {
    if(DEBUG) Serial.println("Drawing game board");
    
    // First, draw the fixed pieces on the board
    for (int y = 0; y < AERO_BOARD_HEIGHT; y++) {
        for (int x = 0; x < AERO_BOARD_WIDTH; x++) {
            if (boardObjects[y][x]) {
                if (board[y][x] == 0) {
                    // Empty cell
                    lv_obj_set_style_bg_color(boardObjects[y][x], lv_color_darken(AERO_COLOR_BACKGROUND, 10), 0);
                } else {
                    // Filled cell
                    lv_obj_set_style_bg_color(boardObjects[y][x], getPieceColor(board[y][x] - 1), 0);
                }
            }
        }
    }
    
    // Then, draw the current falling piece
    if (currentPieceType >= 0) {
        uint16_t piecePattern = pieceDefinitions[currentPieceType][currentRotation];
        lv_color_t color = getPieceColor(currentPieceType);
        
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                if ((piecePattern & (1 << (row * 4 + col))) != 0) {
                    int boardX = currentX + col;
                    int boardY = currentY + row;
                    
                    // Only draw if within board boundaries
                    if (boardX >= 0 && boardX < AERO_BOARD_WIDTH && 
                        boardY >= 0 && boardY < AERO_BOARD_HEIGHT) {
                        if (boardObjects[boardY][boardX]) {
                            lv_obj_set_style_bg_color(boardObjects[boardY][boardX], color, 0);
                        }
                    }
                }
            }
        }
    }
}

// Update the next piece preview
void AeroBlocks::updatePiecePreview() {
    if(DEBUG) Serial.println("Updating next piece preview");
    
    // Clear the preview area
    lv_obj_clean(nextPiecePreview);
    
    // Draw the next piece
    uint16_t piecePattern = pieceDefinitions[nextPieceType][0]; // Use first rotation
    lv_color_t color = getPieceColor(nextPieceType);
    
    // Calculate cell size and offset for preview
    int cellSize = (AERO_GRID_SIZE * AERO_NEXT_PIECE_SIZE) / 5;
    int offsetX = (AERO_GRID_SIZE * AERO_NEXT_PIECE_SIZE - 4 * cellSize) / 2;
    int offsetY = (AERO_GRID_SIZE * AERO_NEXT_PIECE_SIZE - 4 * cellSize) / 2;
    
    // Verschiebe das Vorschau-Tetromino: 1 nach rechts, 2 nach oben
    offsetX -= 1 * cellSize;
    offsetY -= 2 * cellSize;
    
    // Draw each cell of the piece
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if ((piecePattern & (1 << (row * 4 + col))) != 0) {
                lv_obj_t* cell = lv_obj_create(nextPiecePreview);
                lv_obj_set_size(cell, cellSize - 2, cellSize - 2);
                lv_obj_set_pos(cell, offsetX + col * cellSize, offsetY + row * cellSize);
                lv_obj_set_style_bg_color(cell, color, 0);
                lv_obj_set_style_border_width(cell, 1, 0);
                lv_obj_set_style_border_color(cell, lv_color_darken(color, 20), 0);
            }
        }
    }
}

// Update the score, level, and lines display
void AeroBlocks::updateScoreDisplay() {
    if(DEBUG) Serial.println("Updating score display");
    
    // Update the labels with current values
    if (scoreLabel) {
        lv_label_set_text_fmt(scoreLabel, "SCORE\n%d", score);
    }
    
    if (levelLabel) {
        lv_label_set_text_fmt(levelLabel, "LEVEL\n%d", level);
    }
    
    if (linesLabel) {
        lv_label_set_text_fmt(linesLabel, "LINES\n%d", lines);
    }
}

// Move the current piece down one row
void AeroBlocks::moveDown() {
    if(DEBUG) Serial.println("Moving piece down");
    
    // Check if the move down would cause a collision
    if (!checkCollision(currentPieceType, currentRotation, currentX, currentY + 1)) {
        // No collision, move the piece down
        currentY++;
        drawBoard();
    } else {
        // Collision detected, place the piece on the board
        placePiece();
        
        // Check for completed lines
        int linesCleared = clearCompletedLines();
        
        if (linesCleared > 0) {
            // Update score based on lines cleared
            int scoreIncrease = calculateScore(linesCleared, level);
            score += scoreIncrease;
            
            // Update lines count
            lines += linesCleared;
            
            // Check for level up
            if (lines >= linesForNextLevel) {
                level++;
                linesForNextLevel += 10; // Increase lines needed for next level
                
                // Adjust drop speed for the new level
                calculateDropInterval();
                
                if(DEBUG) Serial.printf("Level up! Now level %d\n", level);
            }
            
            // Update score display
            updateScoreDisplay();
        }
        
        // Generate new piece
        generateNextPiece();
        
        // Check if the new piece immediately collides (game over)
        if (checkCollision(currentPieceType, currentRotation, currentX, currentY)) {
            if(DEBUG) Serial.println("Game over!");
            
            // Set game state to game over
            gameState = AERO_STATE_GAME_OVER;
            
            // Show game over screen
            createGameOverScreen();
        }
    }
}

// Check if a piece would collide at the given position
bool AeroBlocks::checkCollision(int pieceType, int rotation, int x, int y) {
    // Get the pattern for the current piece and rotation
    uint16_t pattern = pieceDefinitions[pieceType][rotation];
    
    // Check each cell of the piece
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            // If this cell is part of the piece
            if ((pattern & (1 << (row * 4 + col))) != 0) {
                int boardX = x + col;
                int boardY = y + row;
                
                // Check if out of bounds
                if (boardX < 0 || boardX >= AERO_BOARD_WIDTH || boardY >= AERO_BOARD_HEIGHT) {
                    return true; // Collision with board boundary
                }
                
                // Check if collides with a placed piece
                // But only check if the cell is actually on the board (y >= 0)
                if (boardY >= 0 && board[boardY][boardX] != 0) {
                    return true; // Collision with placed piece
                }
            }
        }
    }
    
    // No collision
    return false;
}

// Generate a new piece
void AeroBlocks::generateNextPiece() {
    if(DEBUG) Serial.println("Generating next piece");
    
    // Set current piece to next piece
    currentPieceType = nextPieceType;
    
    // Generate a new next piece
    nextPieceType = rand() % AERO_PIECE_COUNT;
    
    // Reset position and rotation
    currentRotation = 0;
    currentX = (AERO_BOARD_WIDTH / 2) - 2; // Center the piece
    currentY = -2; // Start just above the visible board
    
    // Update the preview display
    updatePiecePreview();
    
    // Draw the board with the new piece
    drawBoard();
}

// Place the current piece on the board
void AeroBlocks::placePiece() {
    if(DEBUG) Serial.println("Placing piece on board");
    
    uint16_t pattern = pieceDefinitions[currentPieceType][currentRotation];
    
    // For each cell in the piece
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            // If this cell is part of the piece
            if ((pattern & (1 << (row * 4 + col))) != 0) {
                int boardX = currentX + col;
                int boardY = currentY + row;
                
                // Only place if the cell is on the board
                if (boardX >= 0 && boardX < AERO_BOARD_WIDTH && 
                    boardY >= 0 && boardY < AERO_BOARD_HEIGHT) {
                    // Store the piece type + 1 (0 is empty)
                    board[boardY][boardX] = currentPieceType + 1;
                }
            }
        }
    }
}

// Check for and clear completed lines
int AeroBlocks::clearCompletedLines() {
    if(DEBUG) Serial.println("Checking for completed lines");
    
    int linesCleared = 0;
    
    // Check each row from bottom to top
    for (int y = AERO_BOARD_HEIGHT - 1; y >= 0; y--) {
        bool isRowComplete = true;
        
        // Check if row is complete
        for (int x = 0; x < AERO_BOARD_WIDTH; x++) {
            if (board[y][x] == 0) {
                isRowComplete = false;
                break;
            }
        }
        
        // If row is complete
        if (isRowComplete) {
            linesCleared++;
            
            // Shift all rows above down
            for (int copyY = y; copyY > 0; copyY--) {
                for (int x = 0; x < AERO_BOARD_WIDTH; x++) {
                    board[copyY][x] = board[copyY - 1][x];
                }
            }
            
            // Clear the top row
            for (int x = 0; x < AERO_BOARD_WIDTH; x++) {
                board[0][x] = 0;
            }
            
            // Since we've moved all rows down, we need to check this row again
            y++;
        }
    }
    
    // If lines were cleared, redraw the board
    if (linesCleared > 0) {
        drawBoard();
        if(DEBUG) Serial.printf("Cleared %d lines\n", linesCleared);
    }
    
    return linesCleared;
}

// Calculate score based on lines cleared and level
int AeroBlocks::calculateScore(int linesCleared, int currentLevel) {
    // Classic Tetris scoring
    int baseScore = 0;
    
    switch (linesCleared) {
        case 1: baseScore = 40; break;   // Single line
        case 2: baseScore = 100; break;  // Double line
        case 3: baseScore = 300; break;  // Triple line
        case 4: baseScore = 1200; break; // Tetris!
    }
    
    // Multiply by level for difficulty scaling
    return baseScore * currentLevel;
}

// Calculate drop interval based on level
void AeroBlocks::calculateDropInterval() {
    // Base speed of 1000ms (1 second per drop)
    // Each level increases speed by reducing the interval
    dropInterval = 1000 - (level - 1) * 100;
    
    // Ensure minimum drop speed
    if (dropInterval < 100) {
        dropInterval = 100; // Minimum 100ms between drops
    }
    
    if(DEBUG) Serial.printf("Drop interval: %d ms\n", dropInterval);
}

// Get the color for a piece type
lv_color_t AeroBlocks::getPieceColor(int pieceType) {
    switch (pieceType) {
        case AERO_FIGHTER:    return AERO_COLOR_FIGHTER;
        case AERO_BOMBER:     return AERO_COLOR_BOMBER;
        case AERO_STEALTH:    return AERO_COLOR_STEALTH;
        case AERO_CARGO:      return AERO_COLOR_CARGO;
        case AERO_HELICOPTER: return AERO_COLOR_HELICOPTER;
        case AERO_SEAPLANE:   return AERO_COLOR_SEAPLANE;
        case AERO_BIPLANE:    return AERO_COLOR_BIPLANE;
        default:              return AERO_COLOR_TEXT;
    }
}

// Callback für den Heartbeat-Timer
void AeroBlocks::heartbeatCallback(lv_timer_t* timer) {
    AeroBlocks* game = static_cast<AeroBlocks*>(timer->user_data);
    game->sendHeartbeat();
}

// Sendet ein Lebenszeichen an den Controller
void AeroBlocks::sendHeartbeat() {
    // Nur senden, wenn das Spiel aktiv ist und UART-Kommunikation verfügbar ist
    if ((gameState == AERO_STATE_PLAYING || gameState == AERO_STATE_PAUSED) && uartCom != nullptr) {
        // Nachrichtenformat: "AB:state,score,level,lines"
        // AB = AeroBlocks, state = 1 für spielend, 0 für pausiert
        char message[64];
        int playing = (gameState == AERO_STATE_PLAYING) ? 1 : 0;
        snprintf(message, sizeof(message), "AB:%d,%d,%d,%d", playing, score, level, lines);
        
        // Senden über UART-Kommunikation mit sendData statt Message
        // Verwende COM_ID_STATUS (oder einen anderen passenden ID) für Heartbeat-Nachrichten
        uartCom->sendData('W', COM_ID_STATUS, message, false);
        
        if(DEBUG) Serial.printf("Heartbeat sent: %s\n", message);
    }
}

// Neue Methode zum Setzen der UART-Kommunikation
void AeroBlocks::setUartCommunication(UartCommunication* uart) {
    uartCom = uart;
    if(DEBUG) Serial.println("UART Communication set for AeroBlocks Game");
}
