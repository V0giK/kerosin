/*
 * Hauptprogramm zur Steuerung einer Arduino-basierten Tanklösung
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

#include "lovyanGfxSetup.h" // Hier wird LovyanGFX Setup inkludiert
#include <lvgl.h>

#define BLOCK_SIZE 20  // Blockgröße in Pixel
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

// Tetriminos: J, L, O, S, Z, T, I
const uint8_t TETROMINOS[7][4] = {
    {0x1C, 0x14, 0x14, 0x00},  // T-Form
    {0x1C, 0x04, 0x04, 0x04},  // L-Form
    {0x0E, 0x04, 0x04, 0x00},  // J-Form
    {0x0C, 0x0A, 0x0A, 0x00},  // S-Form
    {0x06, 0x06, 0x00, 0x00},  // Z-Form
    {0x1E, 0x00, 0x00, 0x00},  // O-Form
    {0x1C, 0x14, 0x14, 0x14}   // I-Form
};



// const uint16_t TETROMINOS[7][4] = {
//     {0x0F, 0x1111, 0x0F, 0x1111}, // I
//     {0x0730, 0x0470, 0x0730, 0x0470}, // S
//     {0x0E4E, 0x0227, 0x0E4E, 0x0227}, // Z
//     {0x0730, 0x0222, 0x0730, 0x0222}, // T
//     {0x0E0E, 0x0440, 0x0E0E, 0x0440}, // L
//     {0x0E08, 0x0440, 0x0E08, 0x0440}, // J
//     {0x0660, 0x0660, 0x0660, 0x0660}  // O
// };

class Tetris {
public:
    Tetris() {
        isGameRunning = false;
        originalScreen = nullptr;
    }

    void init() {
        for (int i = 0; i < BOARD_HEIGHT; ++i) {
            for (int j = 0; j < BOARD_WIDTH; ++j) {
                board[i][j] = 0;  // 0 bedeutet leer
            }
        }

        // Erstelle die Anzeige für das Spielfeld
        //board_obj = lv_obj_create(lv_scr_act());  // Verwende den aktuellen Bildschirm
        board_obj = lv_obj_create(NULL); // Create a new screen for the game
        lv_obj_set_size(board_obj, BOARD_WIDTH * BLOCK_SIZE, BOARD_HEIGHT * BLOCK_SIZE);
        lv_obj_align(board_obj, LV_ALIGN_CENTER, 0, 0);

        // Event-Handler für Touchscreen-Steuerung
        lv_obj_add_event_cb(board_obj, board_event_cb, LV_EVENT_CLICKED, this);
    }

    void startGame() {
        originalScreen = lv_scr_act();  // Speichere den aktuellen Bildschirm
        init();

        lv_scr_load(board_obj);  // Lade das Spielfeld-Objekt als aktuellen Bildschirm
        isGameRunning = true;
        spawnPiece();  // Erstelle das erste Tetrimino
        updateBoard();
    }

    void stopGame() {
        isGameRunning = false;
        
        // 2 Sekunden warten und dann zurück zum Ausgangsbildschirm wechseln
        lv_timer_create(task_switch_back, 2000, originalScreen); // Übergabe von "this" (die aktuelle Instanz) als user_data
    }

    static void task_switch_back(lv_timer_t* timer) {
        // Zugriff auf den ursprünglichen Bildschirm, der als Parameter übergeben wurde
        lv_obj_t* originalScreen = (lv_obj_t*)timer->user_data; // Hole den Bildschirm aus den Benutzerdaten
        lv_scr_load(originalScreen);  // Lade den ursprünglichen Bildschirm zurück
    }

    void update() {
        if (!isGameRunning) return;

        if (gameOver()) {
            stopGame();
            return;
        }

        // Bewege das Tetrimino nach unten
        if (canMove(0, 1)) {
            y_pos++;
        } else {
            // Füge das Tetrimino zum Spielfeld hinzu
            mergePiece();
            // Überprüfe auf vollständige Linien
            removeFullLines();
            spawnPiece(); // Erstelle ein neues Tetrimino
        }

        updateBoard();
    }

private:
    lv_obj_t* board_obj;
    lv_obj_t* originalScreen;  // Speichert den ursprünglichen Bildschirm
    uint8_t board[BOARD_HEIGHT][BOARD_WIDTH];
    uint8_t currentPiece[4];
    int currentRotation = 0;
    int x_pos = 4;
    int y_pos = 0;
    bool isGameRunning;

    void spawnPiece() {
        // Zufällige Auswahl eines Tetriminos aus dem Array
        int randomPiece = esp_random() % 7; // Es gibt 7 Tetriminos (0-6)

        // Kopiere das gewählte Tetrimino in das aktuelle Tetrimino (currentPiece)
        memcpy(currentPiece, TETROMINOS[randomPiece], sizeof(TETROMINOS[randomPiece]));

        // Setze die Startposition (normalerweise in der Mitte des Spielfelds)
        x_pos = BOARD_WIDTH / 2 - 2;  // Wir setzen die Mitte der x-Achse (die Form ist 4x4, daher -2)
        y_pos = 0;  // Start in der obersten Reihe

        // Wenn du das Tetrimino in einer gedrehten Position starten willst, kannst du hier die Drehung einfügen:
        // rotatePiece();
        
        // Zeichne das Tetrimino auf das Spielfeld
        updateBoard();
    }


    bool canMove(int dx, int dy) {
        // Überprüfe, ob das Tetrimino sich an der neuen Position bewegen kann
        for (int i = 0; i < 4; ++i) {
            int x = (currentPiece[i] % 4) + x_pos + dx;
            int y = (currentPiece[i] / 4) + y_pos + dy;

            if (x < 0 || x >= BOARD_WIDTH || y >= BOARD_HEIGHT || (y >= 0 && board[y][x] != 0)) {
                return false;
            }
        }
        return true;
    }

    bool gameOver() {
        // Überprüfe, ob das Spiel vorbei ist (beispielsweise, wenn das Tetrimino den oberen Rand berührt)
        for (int i = 0; i < 4; ++i) {
            int x = (currentPiece[i] % 4) + x_pos;
            int y = (currentPiece[i] / 4) + y_pos;
            if (y < 0) {
                return true; // Spiel vorbei, wenn ein Tetrimino die oberste Reihe erreicht
            }
        }
        return false;
    }

    void mergePiece() {
        // Füge das aktuelle Tetrimino zum Spielfeld hinzu
        for (int i = 0; i < 4; ++i) {
            int x = (currentPiece[i] % 4) + x_pos;
            int y = (currentPiece[i] / 4) + y_pos;
            if (y >= 0 && x >= 0 && x < BOARD_WIDTH && y < BOARD_HEIGHT) {
                board[y][x] = 1;  // Block auf dem Spielfeld
            }
        }
    }

    void removeFullLines() {
        // Überprüfe und entferne vollständige Linien
        for (int i = 0; i < BOARD_HEIGHT; ++i) {
            bool fullLine = true;
            for (int j = 0; j < BOARD_WIDTH; ++j) {
                if (board[i][j] == 0) {
                    fullLine = false;
                    break;
                }
            }
            if (fullLine) {
                // Verschiebe alle Linien nach unten
                for (int k = i; k > 0; --k) {
                    for (int j = 0; j < BOARD_WIDTH; ++j) {
                        board[k][j] = board[k - 1][j];
                    }
                }
                // Leere die oberste Linie
                for (int j = 0; j < BOARD_WIDTH; ++j) {
                    board[0][j] = 0;
                }
            }
        }
    }

    void updateBoard() {
        // Zeichne das Spielfeld
        lv_obj_clean(board_obj);

        for (int y = 0; y < BOARD_HEIGHT; ++y) {
            for (int x = 0; x < BOARD_WIDTH; ++x) {
                if (board[y][x] != 0) {
                    lv_obj_t* block = lv_obj_create(board_obj);
                    lv_obj_set_size(block, BLOCK_SIZE, BLOCK_SIZE);
                    lv_obj_align(block, LV_ALIGN_TOP_LEFT, x * BLOCK_SIZE, y * BLOCK_SIZE);
                    lv_obj_set_style_bg_color(block, lv_color_hex(0x00FF00), LV_PART_MAIN);  // Farbe für das Spielfeld
                }
            }
        }

        // Zeichne das aktuelle Tetrimino
printCurrentPiece();
        // Zeichne das aktuelle Tetrimino
        for (int y_offset = 0; y_offset < 4; ++y_offset) {
            for (int x_offset = 0; x_offset < 4; ++x_offset) {
                if (currentPiece[y_offset] & (0x80 >> x_offset)) {  // Überprüfen, ob der Block gesetzt ist
                    // Berechne die absolute Position des Blocks im Spielfeld
                    int x = x_pos + x_offset;
                    int y = y_pos + y_offset;

                    if (y >= 0) {  // Überprüfen, dass die Position innerhalb des Spielfelds liegt
                        lv_obj_t* block = lv_obj_create(board_obj);
                        lv_obj_set_size(block, BLOCK_SIZE, BLOCK_SIZE);
                        lv_obj_align(block, LV_ALIGN_TOP_LEFT, x * BLOCK_SIZE, y * BLOCK_SIZE);
                        lv_obj_set_style_bg_color(block, lv_color_hex(0xFF0000), LV_PART_MAIN);  // Farbe für das Tetrimino
                    }
                }
            }
        }
    }

    static void board_event_cb(lv_event_t* e) {
        Tetris* game = (Tetris*)lv_event_get_user_data(e);

        if (!game->isGameRunning) {
            return;
        }

        lv_point_t touch_point;
        lv_indev_get_point(lv_indev_get_act(), &touch_point);

        int screen_width = lv_obj_get_width(game->board_obj);
        int screen_height = lv_obj_get_height(game->board_obj);

        if (touch_point.x < screen_width / 3) {
            game->moveLeft();
        } else if (touch_point.x > 2 * screen_width / 3) {
            game->moveRight();
        } else {
            game->rotatePiece();
        }

        game->update();  // Tetrimino nach unten bewegen
    }

    void moveLeft() {
        if (canMove(-1, 0)) {
            x_pos--;
        }
    }

    void moveRight() {
        if (canMove(1, 0)) {
            x_pos++;
        }
    }

    void rotatePiece() {
        uint8_t temp[4];

        // Kopiere das aktuelle Tetrimino in ein temporäres Array
        for (int i = 0; i < 4; ++i) {
            temp[i] = currentPiece[i];
        }

        // Drehe das Tetrimino um 90 Grad im Uhrzeigersinn
        for (int i = 0; i < 4; ++i) {
            currentPiece[i] = temp[(3 - i) % 4];  // Umrechnen der 2D-Koordinaten in den 1D-Index
        }

        // Überprüfe, ob das gedrehte Tetrimino auf dem Spielfeld Platz hat
        if (!canMove(0, 0)) {
            // Wenn Drehung nicht geht, mache die Rotation rückgängig (drehe 90 Grad gegen den Uhrzeigersinn)
            // Dies erreicht man, indem wir die Matrix nochmal im Uhrzeigersinn rotieren, aber 3-mal
            for (int i = 0; i < 3; ++i) {
                // Zurückdrehen durch Rotation um 270 Grad (im Uhrzeigersinn)
                uint8_t temp[4];
                // Kopiere die Matrix wieder
                for (int i = 0; i < 4; ++i) {
                    temp[i] = currentPiece[i];
                }

                // Drehe um 90 Grad im Uhrzeigersinn (für 270 Grad rückwärts)
                for (int i = 0; i < 4; ++i) {
                    currentPiece[i] = temp[(3 - i) % 4];
                }
            }
        }
    }



    // void rotate() {
    //     rotatePiece();
    //     if (!canMove(0, 0)) {
    //         rotatePiece(); // Wenn Drehung nicht geht, zurückdrehen
    //         rotatePiece();
    //         rotatePiece();
    //     }
    // }

    void printCurrentPiece() {
        Serial.println("Aktuelles Tetrimino:");
        for (int i = 0; i < 4; i++) {
            Serial.print("Zeile ");
            Serial.print(i);
            Serial.print(": ");
            for (int j = 3; j >= 0; j--) {
                if ((currentPiece[i] >> j) & 0x01) {
                    Serial.print("1");
                } else {
                    Serial.print("0");
                }
            }
            Serial.println();
        }
    }

};

// // Global Tetris-Objekt
// Tetris* tetrisGame = nullptr;

// void startTetrisGame() {
//     if (tetrisGame != nullptr) {
//         tetrisGame->startGame();
//     }
// }

// void stopTetrisGame() {
//     if (tetrisGame != nullptr) {
//         tetrisGame->stopGame();
//     }
// }

// void setup() {
//     lv_init();
//     lvglSetup();
//     ui_init();

//     tetrisGame = new Tetris();  // Tetris ohne Parameter erstellen
// }

// void loop() {
//     lv_task_handler();
//     ui_tick();
// }
