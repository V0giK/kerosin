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
    void reset(); // New method to completely reset the game state

private:
    // Game constants
    static const int GRID_SIZE = 20;
    static const int GRID_WIDTH = 800 / GRID_SIZE;
    static const int GRID_HEIGHT = 480 / GRID_SIZE;
    static const int MAX_SNAKE_LENGTH = 200;
    
    // Game states
    enum GameState {
        STATE_START_SCREEN,
        STATE_PLAYING,
        STATE_GAME_OVER
    };
    
    // Difficulty levels
    enum Difficulty {
        EASY = 0,
        MEDIUM = 1,
        HARD = 2
    };

    // UI elements
    lv_obj_t *scr;
    lv_obj_t *snake_body[MAX_SNAKE_LENGTH];
    lv_obj_t *food;
    lv_obj_t *score_label;
    lv_obj_t *prev_screen; // Store the previous screen
    
    // Game state tracking
    int snake_length;
    int direction; // 0: right, 1: down, 2: left, 3: up
    bool game_running;
    GameState current_state;
    Difficulty current_difficulty;
    
    // Timers
    lv_timer_t *game_timer;
    lv_timer_t *game_over_timer;
    lv_timer_t *animation_timer;
    
    // Game parameters
    int speed; // Speed in milliseconds for the timer
    int score; // Score to track how many foods eaten
    int high_score; // Track high score
    
    // Visual styles
    lv_style_t snake_style; // Style for the snake
    lv_style_t food_style;  // Style for the food
    lv_style_t title_style; // Style for title text
    lv_style_t button_style; // Style for buttons
    
    // Start screen elements
    lv_obj_t *start_container;
    lv_obj_t *title_label;
    lv_obj_t *high_score_label;
    lv_obj_t *difficulty_buttons[3];
    lv_obj_t *start_button;
    lv_obj_t *demo_snake[5]; // For animated snake on start screen

    // Add head position tracking
    int head_x;
    int head_y;
    
    // Add debug flag
    bool debugEnabled = false;

    // Add a paused state flag
    bool paused = false;

    // Game callbacks
    static void game_timer_cb(lv_timer_t *t) {
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
    
    static void start_button_cb(lv_event_t *e) {
        SnakeGame *self = static_cast<SnakeGame *>(lv_event_get_user_data(e));
        self->start_game();
    }
    
    static void difficulty_button_cb(lv_event_t *e) {
        SnakeGame *self = static_cast<SnakeGame *>(lv_event_get_user_data(e));
        lv_obj_t *btn = (lv_obj_t*)lv_event_get_target(e);
        int difficulty = (int)lv_obj_get_user_data(btn);
        self->set_difficulty((Difficulty)difficulty);
    }
    
    static void animation_timer_cb(lv_timer_t *t) {
        SnakeGame *self = static_cast<SnakeGame *>(t->user_data);
        self->animate_start_screen();
    }

    // New joystick callbacks
    static void joystick_event_cb(lv_event_t *e);
    static void pause_event_cb(lv_event_t *e);

    // Game methods
    void move_snake();
    void change_direction(lv_event_t *e);
    void show_game_over();
    void restore_screen();
    
    // New methods
    void create_start_screen();
    void clean_start_screen();
    void create_game_screen();
    void update_score_display();
    void start_game();
    void set_difficulty(Difficulty diff);
    void animate_start_screen();
    void reset_game();
    void spawn_food();
    bool check_collision(int x, int y);
    lv_color_t get_snake_color(int segment);
    
    // New method for direct touch handling
    void check_touch_input();

    // Helper function to validate direction changes
    bool isValidDirectionChange(int current, int requested);
};
