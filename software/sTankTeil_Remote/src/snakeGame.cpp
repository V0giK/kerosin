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

#include "snakeGame.h"

SnakeGame::SnakeGame() : snake_length(3), direction(0), game_running(false), timer(nullptr), prev_screen(nullptr), game_over_timer(nullptr), speed(500), score(0) {
    //
}

void SnakeGame::start() {
    if (game_running) return; // Prevent starting the game multiple times

    snake_length = 3;
    direction = 0;
    game_running = false;
    timer = nullptr;
    prev_screen = nullptr;
    game_over_timer = nullptr;
    speed = 500;
    score = 0;

    lv_style_init(&snake_style);
    lv_style_set_bg_color(&snake_style, lv_color_hex(0x00FF00)); // Grün für die Schlange
    lv_style_set_radius(&snake_style, 0); // Eckige Blöcke

    lv_style_init(&food_style);
    lv_style_set_bg_color(&food_style, lv_color_hex(0xFF0000)); // Rot für das Essen
    lv_style_set_radius(&food_style, 0); // Eckige Blöcke

    
    game_running = true;
    prev_screen = lv_scr_act(); // Save the current screen

    lv_obj_t *game_screen = lv_obj_create(NULL); // Create a new screen for the game
    lv_scr_load(game_screen); // Load the game screen

    scr = game_screen;

    // Create touch area for direction change
    lv_obj_t *touch_area = lv_obj_create(scr);
    lv_obj_set_size(touch_area, LV_HOR_RES, LV_VER_RES);
    lv_obj_add_event_cb(touch_area, change_direction_cb, LV_EVENT_CLICKED, this);

    // Initialize snake body
    for (int i = 0; i < snake_length; i++) {
        snake_body[i] = lv_obj_create(scr);
        lv_obj_set_size(snake_body[i], GRID_SIZE, GRID_SIZE);
        lv_obj_add_style(snake_body[i], &snake_style, LV_PART_MAIN);
    }

    // Initialize food
    food = lv_obj_create(scr);
    lv_obj_set_size(food, GRID_SIZE, GRID_SIZE);
    lv_obj_add_style(food, &food_style, LV_PART_MAIN);
    lv_obj_set_pos(food, (esp_random() % GRID_WIDTH) * GRID_SIZE, (esp_random() % GRID_HEIGHT) * GRID_SIZE);

    // Start game timer
    timer = lv_timer_create(timer_cb, speed, this);
}

void SnakeGame::stop() {
    if (!game_running) return; // Prevent stopping the game if it's not running

    game_running = false;
    if (timer) {
        lv_timer_del(timer);
        timer = nullptr;
    }

    // Show "Game Over" and start a timer to restore the previous screen after 1 second
    show_game_over();
    game_over_timer = lv_timer_create(game_over_timer_cb, 1000, this);
}


void SnakeGame::move_snake() {
    static int head_x = GRID_WIDTH / 2;
    static int head_y = GRID_HEIGHT / 2;

    // Update head position
    switch (direction) {
        case 0: head_x++; break; // Right
        case 1: head_y++; break; // Down
        case 2: head_x--; break; // Left
        case 3: head_y--; break; // Up
    }

    // Check collision with walls
    if (head_x < 0 || head_x >= GRID_WIDTH || head_y < 0 || head_y >= GRID_HEIGHT) {
        stop();
        return;
    }

    // Check collision with itself
    for (int i = 1; i < snake_length; i++) {
        if (lv_obj_get_x(snake_body[i]) / GRID_SIZE == head_x &&
            lv_obj_get_y(snake_body[i]) / GRID_SIZE == head_y) {
            stop();
            return;
        }
    }

    // Move body
    for (int i = snake_length - 1; i > 0; i--) {
        lv_obj_set_pos(snake_body[i], lv_obj_get_x(snake_body[i-1]), lv_obj_get_y(snake_body[i-1]));
    }
    lv_obj_set_pos(snake_body[0], head_x * GRID_SIZE, head_y * GRID_SIZE);

    // Check food collision
    if (lv_obj_get_x(snake_body[0]) == lv_obj_get_x(food) &&
        lv_obj_get_y(snake_body[0]) == lv_obj_get_y(food)) {
        score++;
        snake_length++;
        lv_obj_t *new_body = lv_obj_create(scr);
        lv_obj_set_size(new_body, GRID_SIZE, GRID_SIZE);
        lv_obj_add_style(new_body, &snake_style, LV_PART_MAIN);
        snake_body[snake_length - 1] = new_body;

        lv_obj_set_pos(food, (esp_random() % GRID_WIDTH) * GRID_SIZE, (esp_random() % GRID_HEIGHT) * GRID_SIZE);

        // Increase speed
        if (speed > 100) { // Minimum speed limit
            speed -= 20; // Increase speed by reducing the timer interval
            lv_timer_set_period(timer, speed); // Update the timer interval
        }
    }
}

void SnakeGame::change_direction(lv_event_t *e) {
    lv_point_t p;
    lv_indev_get_point(lv_indev_get_act(), &p);

    // if (p.x > 400 && p.y > 240) direction = 1; // Down
    // else if (p.x > 400) direction = 0; // Right
    // else if (p.y > 240) direction = 2; // Left
    // else direction = 3; // Up

    if (p.y > 380) direction = 1; // Down
    else if (p.y < 100) direction = 3; // Up
    else if (p.x > 400) direction = 0; // Right
    else direction = 2; // Left
}

void SnakeGame::show_game_over() {
    lv_obj_t *label = lv_label_create(scr);
    String text = "Game Over!\nScore: " + String(score);
    lv_label_set_text(label, text.c_str());
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}

void SnakeGame::restore_screen() {
    lv_scr_load(prev_screen); // Restore the previous screen
    if (game_over_timer) {
        lv_timer_del(game_over_timer);
        game_over_timer = nullptr;
    }
}
