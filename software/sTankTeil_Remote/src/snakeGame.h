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

class SnakeGame {
public:
    SnakeGame();
    void start();
    void stop();

private:
    static const int GRID_SIZE = 20;
    static const int GRID_WIDTH = 800 / GRID_SIZE;
    static const int GRID_HEIGHT = 480 / GRID_SIZE;

    lv_obj_t *scr;
    lv_obj_t *snake_body[100];
    lv_obj_t *food;
    int snake_length;
    int direction; // 0: right, 1: down, 2: left, 3: up
    bool game_running;
    lv_timer_t *timer;
    lv_timer_t *game_over_timer; // Timer for the "Game Over" message
    lv_obj_t *prev_screen; // Store the previous screen
    int speed; // Speed in milliseconds for the timer
    int score; // Score to track how many foods eaten

    lv_style_t snake_style; // Style for the snake
    lv_style_t food_style;  // Style for the food

    static void timer_cb(lv_timer_t *t) {
        SnakeGame *self = static_cast<SnakeGame *>(t->user_data);
        self->move_snake();
    }

    static void change_direction_cb(lv_event_t *e) {
        SnakeGame *self = static_cast<SnakeGame *>(lv_event_get_user_data(e));
        self->change_direction(e);
    }

    static void game_over_timer_cb(lv_timer_t *t) {
        SnakeGame *self = static_cast<SnakeGame *>(t->user_data);
        self->restore_screen();
    }

    void move_snake();
    void change_direction(lv_event_t *e);
    void show_game_over();
    void restore_screen();
};
