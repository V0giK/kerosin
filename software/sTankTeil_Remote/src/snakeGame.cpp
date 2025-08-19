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
#include "main.h"  // Include main.h to get access to the tft object
#include "esp_task_wdt.h"  // Include ESP Task Watchdog Timer API

SnakeGame::SnakeGame() : 
    snake_length(3), 
    direction(0), 
    game_running(false), 
    current_state(STATE_START_SCREEN),
    current_difficulty(MEDIUM),
    game_timer(nullptr), 
    game_over_timer(nullptr),
    animation_timer(nullptr),
    prev_screen(nullptr), 
    speed(250), 
    score(0),
    high_score(0),
    head_x(0),  // Initialize head position
    head_y(0),
    debugEnabled(true)  // Enable debug by default
{
    // Initialize styles and game parameters
    Serial.println("Snake game constructor called");
}

void SnakeGame::start() {
    Serial.println("Snake game starting...");
    try {
        if (game_running) {
            Serial.println("Game already running, ignoring start request");
            // Force reset game if it's stuck in running state but not actually visible
            if (scr == nullptr) {
                Serial.println("ERROR: Game was marked as running but screen is null. Forcing reset...");
                reset();
            } else {
                return; // Prevent starting the game multiple times
            }
        }

        // Clear all variables first to ensure no dangling pointers
        for (int i = 0; i < MAX_SNAKE_LENGTH; i++) {
            snake_body[i] = nullptr;
        }
        for (int i = 0; i < 5; i++) {
            demo_snake[i] = nullptr;
        }
        
        food = nullptr;
        score_label = nullptr;
        start_container = nullptr;
        title_label = nullptr;
        high_score_label = nullptr;
        for (int i = 0; i < 3; i++) {
            difficulty_buttons[i] = nullptr;
        }
        start_button = nullptr;

        // Reset game state - make sure all variables are properly initialized
        snake_length = 3;
        direction = 0;
        score = 0;
        high_score = high_score; // Keep high score between sessions
        head_x = 0;
        head_y = 0;
        current_state = STATE_START_SCREEN;
        game_running = true;
        
        // Save previous screen
        prev_screen = lv_scr_act();
        if (!prev_screen) {
            Serial.println("WARNING: No active screen to save");
        } else {
            Serial.println("Previous screen saved successfully");
        }

        // Create new screen for the game
        Serial.println("Creating game screen");
        scr = lv_obj_create(NULL);
        if (!scr) {
            Serial.println("ERROR: Failed to create game screen");
            reset();
            return;
        }
        lv_scr_load(scr);

        // Initialize styles
        Serial.println("Initializing styles");
        lv_style_init(&snake_style);
        lv_style_set_bg_color(&snake_style, lv_color_hex(0x00AA00)); // Green for snake
        lv_style_set_radius(&snake_style, 5); // Rounded blocks

        lv_style_init(&food_style);
        lv_style_set_bg_color(&food_style, lv_color_hex(0xFF0000)); // Red for food
        lv_style_set_radius(&food_style, 10); // Round food

        lv_style_init(&title_style);
        lv_style_set_text_font(&title_style, &lv_font_montserrat_30);
        lv_style_set_text_color(&title_style, lv_color_hex(0xFFFFFF));

        lv_style_init(&button_style);
        lv_style_set_bg_color(&button_style, lv_color_hex(0x2196F3));
        lv_style_set_text_color(&button_style, lv_color_hex(0xFFFFFF));
        lv_style_set_radius(&button_style, 10);
        lv_style_set_border_width(&button_style, 0);
        
        // Show start screen
        Serial.println("Creating start screen");
        create_start_screen();
        
        // Start animation timer with a shorter interval to prevent blocking
        Serial.println("Starting animation timer");
        animation_timer = lv_timer_create(animation_timer_cb, 200, this);
        
        // Reset watchdog to prevent timeout during initialization
        esp_task_wdt_reset();
        
        Serial.println("Snake game started successfully");
    } catch (...) {
        Serial.println("ERROR: Exception in Snake game start()");
        reset(); // Reset the game state on error
    }
}

void SnakeGame::create_start_screen() {
    // Create container for start screen elements
    start_container = lv_obj_create(scr);
    lv_obj_set_size(start_container, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_bg_color(start_container, lv_color_hex(0x111111), LV_PART_MAIN);
    lv_obj_set_style_border_width(start_container, 0, LV_PART_MAIN);
    lv_obj_center(start_container);
    
    // Create title
    title_label = lv_label_create(start_container);
    lv_label_set_text(title_label, "SNAKE GAME");
    lv_obj_add_style(title_label, &title_style, LV_PART_MAIN);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 40);
    
    // Create high score display
    high_score_label = lv_label_create(start_container);
    lv_label_set_text_fmt(high_score_label, "High Score: %d", high_score);
    lv_obj_align(high_score_label, LV_ALIGN_TOP_MID, 0, 100);
    
    // Create difficulty buttons
    const char* diff_names[3] = {"Easy", "Medium", "Hard"};
    for (int i = 0; i < 3; i++) {
        difficulty_buttons[i] = lv_btn_create(start_container);
        lv_obj_set_size(difficulty_buttons[i], 120, 50);
        lv_obj_add_style(difficulty_buttons[i], &button_style, LV_PART_MAIN);
        lv_obj_align(difficulty_buttons[i], LV_ALIGN_CENTER, (i-1) * 150, 0);
        lv_obj_set_user_data(difficulty_buttons[i], (void*)i);
        lv_obj_add_event_cb(difficulty_buttons[i], difficulty_button_cb, LV_EVENT_CLICKED, this);
        
        // Highlight selected difficulty
        if (i == current_difficulty) {
            lv_obj_set_style_bg_color(difficulty_buttons[i], lv_color_hex(0xFF9800), LV_PART_MAIN);
        }
        
        lv_obj_t *label = lv_label_create(difficulty_buttons[i]);
        lv_label_set_text(label, diff_names[i]);
        lv_obj_center(label);
    }
    
    // Create start button
    start_button = lv_btn_create(start_container);
    lv_obj_set_size(start_button, 200, 60);
    lv_obj_add_style(start_button, &button_style, LV_PART_MAIN);
    lv_obj_set_style_bg_color(start_button, lv_color_hex(0x4CAF50), LV_PART_MAIN);
    lv_obj_align(start_button, LV_ALIGN_BOTTOM_MID, 0, -50);
    lv_obj_add_event_cb(start_button, start_button_cb, LV_EVENT_CLICKED, this);
    
    lv_obj_t *label = lv_label_create(start_button);
    lv_label_set_text(label, "START GAME");
    lv_obj_center(label);
    
    // Create animated snake on start screen
    for (int i = 0; i < 5; i++) {
        demo_snake[i] = lv_obj_create(start_container);
        lv_obj_set_size(demo_snake[i], GRID_SIZE, GRID_SIZE);
        lv_obj_add_style(demo_snake[i], &snake_style, LV_PART_MAIN);
        // Position will be set in animation
        lv_obj_set_pos(demo_snake[i], 200 - i * GRID_SIZE, 300);
    }
}

void SnakeGame::animate_start_screen() {
    static int anim_offset = 0;
    
    // Move demo snake segments in a wave pattern
    for (int i = 0; i < 5; i++) {
        int x = 200 - i * GRID_SIZE + anim_offset;
        int y = 300 + sin((anim_offset + i * 3) * 0.1) * 30;
        
        // Wrap around edges
        if (x > LV_HOR_RES) x = -GRID_SIZE;
        
        lv_obj_set_pos(demo_snake[i], x, y);
        
        // Change colors based on position
        lv_color_t color = get_snake_color(i);
        lv_obj_set_style_bg_color(demo_snake[i], color, LV_PART_MAIN);
    }
    
    anim_offset += 5;
}

lv_color_t SnakeGame::get_snake_color(int segment) {
    // Rainbow effect for snake segments
    switch (segment % 5) {
        case 0: return lv_color_hex(0x00AA00); // Green
        case 1: return lv_color_hex(0x00CC00); // Lighter green
        case 2: return lv_color_hex(0x00EE00); // Even lighter green
        case 3: return lv_color_hex(0x00BB00); // Medium green
        case 4: return lv_color_hex(0x009900); // Darker green
        default: return lv_color_hex(0x00AA00);
    }
}

void SnakeGame::set_difficulty(Difficulty diff) {
    current_difficulty = diff;
    
    // Update speed based on difficulty
    switch (diff) {
        case EASY:
            speed = 350; // Slower
            break;
        case MEDIUM:
            speed = 250; // Medium speed
            break;
        case HARD:
            speed = 150; // Faster
            break;
    }
    
    // Update UI to show selected difficulty
    for (int i = 0; i < 3; i++) {
        if (i == diff) {
            lv_obj_set_style_bg_color(difficulty_buttons[i], lv_color_hex(0xFF9800), LV_PART_MAIN);
        } else {
            lv_obj_set_style_bg_color(difficulty_buttons[i], lv_color_hex(0x2196F3), LV_PART_MAIN);
        }
    }
}

void SnakeGame::start_game() {
    // Clean start screen
    clean_start_screen();
    
    // Reset game state
    current_state = STATE_PLAYING;
    reset_game();
    
    // Create game screen elements
    create_game_screen();
    
    // Start game timer
    game_timer = lv_timer_create(game_timer_cb, speed, this);
}

void SnakeGame::clean_start_screen() {
    if (animation_timer) {
        lv_timer_del(animation_timer);
        animation_timer = nullptr;
    }
    
    lv_obj_del(start_container);
}

void SnakeGame::create_game_screen() {
    // Create touch area for direction change
    lv_obj_t *touch_area = lv_obj_create(scr);
    lv_obj_set_size(touch_area, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_bg_opa(touch_area, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(touch_area, 0, LV_PART_MAIN);
    
    // LEFT SIDE CONTROLS
    
    // Left side - UP button
    lv_obj_t *left_up_button = lv_btn_create(scr);
    lv_obj_set_size(left_up_button, 80, 80);
    lv_obj_set_pos(left_up_button, 20, 120);  // Top position on left
    lv_obj_set_style_radius(left_up_button, 40, LV_PART_MAIN);
    lv_obj_set_style_bg_color(left_up_button, lv_color_hex(0x4444FF), LV_PART_MAIN);
    lv_obj_set_user_data(left_up_button, (void*)3); // UP direction = 3
    lv_obj_add_event_cb(left_up_button, joystick_event_cb, LV_EVENT_CLICKED, this);
    
    lv_obj_t *left_up_label = lv_label_create(left_up_button);
    lv_label_set_text(left_up_label, LV_SYMBOL_UP);
    lv_obj_set_style_text_font(left_up_label, &lv_font_montserrat_30, LV_PART_MAIN);
    lv_obj_center(left_up_label);
    
    // Left side - LEFT button
    lv_obj_t *left_button = lv_btn_create(scr);
    lv_obj_set_size(left_button, 80, 80);
    lv_obj_set_pos(left_button, 20, 220);  // Middle position on left
    lv_obj_set_style_radius(left_button, 40, LV_PART_MAIN);
    lv_obj_set_style_bg_color(left_button, lv_color_hex(0x4444FF), LV_PART_MAIN);
    lv_obj_set_user_data(left_button, (void*)2); // LEFT direction = 2
    lv_obj_add_event_cb(left_button, joystick_event_cb, LV_EVENT_CLICKED, this);
    
    lv_obj_t *left_label = lv_label_create(left_button);
    lv_label_set_text(left_label, LV_SYMBOL_LEFT);
    lv_obj_set_style_text_font(left_label, &lv_font_montserrat_30, LV_PART_MAIN);
    lv_obj_center(left_label);
    
    // Left side - DOWN button
    lv_obj_t *left_down_button = lv_btn_create(scr);
    lv_obj_set_size(left_down_button, 80, 80);
    lv_obj_set_pos(left_down_button, 20, 320);  // Bottom position on left
    lv_obj_set_style_radius(left_down_button, 40, LV_PART_MAIN);
    lv_obj_set_style_bg_color(left_down_button, lv_color_hex(0x4444FF), LV_PART_MAIN);
    lv_obj_set_user_data(left_down_button, (void*)1); // DOWN direction = 1
    lv_obj_add_event_cb(left_down_button, joystick_event_cb, LV_EVENT_CLICKED, this);
    
    lv_obj_t *left_down_label = lv_label_create(left_down_button);
    lv_label_set_text(left_down_label, LV_SYMBOL_DOWN);
    lv_obj_set_style_text_font(left_down_label, &lv_font_montserrat_30, LV_PART_MAIN);
    lv_obj_center(left_down_label);
    
    // RIGHT SIDE CONTROLS
    
    // Right side - UP button
    lv_obj_t *right_up_button = lv_btn_create(scr);
    lv_obj_set_size(right_up_button, 80, 80);
    lv_obj_set_pos(right_up_button, LV_HOR_RES - 100, 120);  // Top position on right
    lv_obj_set_style_radius(right_up_button, 40, LV_PART_MAIN);
    lv_obj_set_style_bg_color(right_up_button, lv_color_hex(0x4444FF), LV_PART_MAIN);
    lv_obj_set_user_data(right_up_button, (void*)3); // UP direction = 3
    lv_obj_add_event_cb(right_up_button, joystick_event_cb, LV_EVENT_CLICKED, this);
    
    lv_obj_t *right_up_label = lv_label_create(right_up_button);
    lv_label_set_text(right_up_label, LV_SYMBOL_UP);
    lv_obj_set_style_text_font(right_up_label, &lv_font_montserrat_30, LV_PART_MAIN);
    lv_obj_center(right_up_label);
    
    // Right side - RIGHT button
    lv_obj_t *right_button = lv_btn_create(scr);
    lv_obj_set_size(right_button, 80, 80);
    lv_obj_set_pos(right_button, LV_HOR_RES - 100, 220);  // Middle position on right
    lv_obj_set_style_radius(right_button, 40, LV_PART_MAIN);
    lv_obj_set_style_bg_color(right_button, lv_color_hex(0x4444FF), LV_PART_MAIN);
    lv_obj_set_user_data(right_button, (void*)0); // RIGHT direction = 0
    lv_obj_add_event_cb(right_button, joystick_event_cb, LV_EVENT_CLICKED, this);
    
    lv_obj_t *right_label = lv_label_create(right_button);
    lv_label_set_text(right_label, LV_SYMBOL_RIGHT);
    lv_obj_set_style_text_font(right_label, &lv_font_montserrat_30, LV_PART_MAIN);
    lv_obj_center(right_label);
    
    // Right side - DOWN button
    lv_obj_t *right_down_button = lv_btn_create(scr);
    lv_obj_set_size(right_down_button, 80, 80);
    lv_obj_set_pos(right_down_button, LV_HOR_RES - 100, 320);  // Bottom position on right
    lv_obj_set_style_radius(right_down_button, 40, LV_PART_MAIN);
    lv_obj_set_style_bg_color(right_down_button, lv_color_hex(0x4444FF), LV_PART_MAIN);
    lv_obj_set_user_data(right_down_button, (void*)1); // DOWN direction = 1
    lv_obj_add_event_cb(right_down_button, joystick_event_cb, LV_EVENT_CLICKED, this);
    
    lv_obj_t *right_down_label = lv_label_create(right_down_button);
    lv_label_set_text(right_down_label, LV_SYMBOL_DOWN);
    lv_obj_set_style_text_font(right_down_label, &lv_font_montserrat_30, LV_PART_MAIN);
    lv_obj_center(right_down_label);
    
    // Also keep the original touch area for touch anywhere functionality
    lv_obj_add_event_cb(touch_area, change_direction_cb, LV_EVENT_PRESSED, this);
    
    // Create score display
    score_label = lv_label_create(scr);
    lv_obj_set_style_text_font(score_label, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_obj_set_style_text_color(score_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_label_set_text_fmt(score_label, "Score: %d", score);
    lv_obj_align(score_label, LV_ALIGN_TOP_RIGHT, -10, 10);
    
    // Create game boundary
    lv_obj_t *border = lv_obj_create(scr);
    lv_obj_set_size(border, GRID_WIDTH * GRID_SIZE + 2, GRID_HEIGHT * GRID_SIZE + 2);
    lv_obj_set_style_bg_opa(border, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(border, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(border, lv_color_hex(0x555555), LV_PART_MAIN);
    lv_obj_center(border);
    
    // Initialize snake body
    for (int i = 0; i < snake_length; i++) {
        snake_body[i] = lv_obj_create(scr);
        lv_obj_set_size(snake_body[i], GRID_SIZE - 2, GRID_SIZE - 2);
        lv_obj_add_style(snake_body[i], &snake_style, LV_PART_MAIN);
        lv_obj_set_style_bg_color(snake_body[i], get_snake_color(i), LV_PART_MAIN);
        
        // Initial snake position
        int x = (GRID_WIDTH / 2 - i) * GRID_SIZE;
        int y = GRID_HEIGHT / 2 * GRID_SIZE;
        lv_obj_set_pos(snake_body[i], x, y);
        
        // Set head position for the first segment
        if (i == 0) {
            head_x = x;
            head_y = y;
            if (debugEnabled) {
                Serial.printf("Initial head position: (%d, %d)\n", head_x, head_y);
            }
        }
    }
    
    // Create food
    food = lv_obj_create(scr);
    lv_obj_set_size(food, GRID_SIZE - 2, GRID_SIZE - 2);
    lv_obj_add_style(food, &food_style, LV_PART_MAIN);
    spawn_food();
}

void SnakeGame::spawn_food() {
    int x, y;
    bool valid_position;
    
    // Find a position for food that doesn't overlap with snake
    do {
        valid_position = true;
        x = (esp_random() % (GRID_WIDTH - 2) + 1) * GRID_SIZE;
        y = (esp_random() % (GRID_HEIGHT - 2) + 1) * GRID_SIZE;
        
        // Check collision with snake
        if (check_collision(x, y)) {
            valid_position = false;
        }
    } while (!valid_position);
    
    lv_obj_set_pos(food, x, y);
}

bool SnakeGame::check_collision(int x, int y) {
    // Check if position collides with snake
    for (int i = 0; i < snake_length; i++) {
        if (lv_obj_get_x(snake_body[i]) == x && lv_obj_get_y(snake_body[i]) == y) {
            return true;
        }
    }
    return false;
}

void SnakeGame::reset_game() {
    snake_length = 3;
    direction = 0; // Start moving right
    score = 0;
    
    // Initialize head position
    head_x = GRID_WIDTH / 2 * GRID_SIZE;
    head_y = GRID_HEIGHT / 2 * GRID_SIZE;
    
    if (debugEnabled) {
        Serial.printf("Game reset: head position set to (%d, %d)\n", head_x, head_y);
    }
}

void SnakeGame::move_snake() {
    // Reset watchdog as this may be a long-running operation
    esp_task_wdt_reset();
    
    // Check if touch is active and directly handle the direction change
    check_touch_input();
    
    // Use member variables instead of static locals
    // Remove the static keyword that was here

    // Update head position
    switch (direction) {
        case 0: head_x += GRID_SIZE; break; // Right
        case 1: head_y += GRID_SIZE; break; // Down
        case 2: head_x -= GRID_SIZE; break; // Left
        case 3: head_y -= GRID_SIZE; break; // Up
    }

    // Check collision with walls
    if (head_x < 0 || head_x >= GRID_WIDTH * GRID_SIZE || 
        head_y < 0 || head_y >= GRID_HEIGHT * GRID_SIZE) {
        show_game_over();
        return;
    }

    // Check collision with itself
    for (int i = 1; i < snake_length; i++) {
        if (lv_obj_get_x(snake_body[i]) == head_x && 
            lv_obj_get_y(snake_body[i]) == head_y) {
            show_game_over();
            return;
        }
    }

    // Move body segments
    for (int i = snake_length - 1; i > 0; i--) {
        lv_obj_set_pos(snake_body[i], lv_obj_get_x(snake_body[i-1]), lv_obj_get_y(snake_body[i-1]));
        
        // Update segment colors for rainbow effect
        lv_obj_set_style_bg_color(snake_body[i], get_snake_color((i + score) % 5), LV_PART_MAIN);
    }
    
    // Move head
    lv_obj_set_pos(snake_body[0], head_x, head_y);
    lv_obj_set_style_bg_color(snake_body[0], get_snake_color(score % 5), LV_PART_MAIN);

    // Check food collision
    if (head_x == lv_obj_get_x(food) && head_y == lv_obj_get_y(food)) {
        // Increase score
        score++;
        update_score_display();
        
        // Grow snake
        if (snake_length < MAX_SNAKE_LENGTH) {
            snake_body[snake_length] = lv_obj_create(scr);
            lv_obj_set_size(snake_body[snake_length], GRID_SIZE - 2, GRID_SIZE - 2);
            lv_obj_add_style(snake_body[snake_length], &snake_style, LV_PART_MAIN);
            
            // Position at end of snake
            int tail_x = lv_obj_get_x(snake_body[snake_length - 1]);
            int tail_y = lv_obj_get_y(snake_body[snake_length - 1]);
            lv_obj_set_pos(snake_body[snake_length], tail_x, tail_y);
            
            snake_length++;
        }
        
        // Generate new food
        spawn_food();
        
        // Increase speed slightly as score increases
        if (speed > 80) {
            speed -= 2;
            lv_timer_set_period(game_timer, speed);
        }
    }
}

void SnakeGame::update_score_display() {
    lv_label_set_text_fmt(score_label, "Score: %d", score);
}

// Helper method to check if direction change is valid
bool SnakeGame::isValidDirectionChange(int current, int requested) {
    // Can't do a 180-degree turn (would immediately hit own body)
    // 0: right, 1: down, 2: left, 3: up
    if ((current == 0 && requested == 2) ||  // Right -> Left
        (current == 2 && requested == 0) ||  // Left -> Right
        (current == 1 && requested == 3) ||  // Down -> Up
        (current == 3 && requested == 1)) {  // Up -> Down
        return false;
    }
    return true;
}

void SnakeGame::change_direction(lv_event_t *e) {
    Serial.println("Touch event received");
    
    if (current_state != STATE_PLAYING) {
        Serial.println("Ignoring touch - not in playing state");
        return;
    }
    
    lv_point_t p;
    lv_indev_get_point(lv_indev_get_act(), &p);
    
    // Always print touch position for debugging
    Serial.printf("Touch detected at (%d, %d)\n", p.x, p.y);
    
    // Better direction control using screen quadrants
    int screen_width = LV_HOR_RES;
    int screen_height = LV_VER_RES;
    int new_direction = direction;
    
    // Determine new direction based on touch position
    if (p.y < p.x && p.y < screen_height - p.x) {
        // Top quadrant - UP
        new_direction = 3;
    } else if (p.y >= p.x && p.y < screen_height - p.x) {
        // Left quadrant - LEFT
        new_direction = 2;
    } else if (p.y >= p.x && p.y >= screen_height - p.x) {
        // Bottom quadrant - DOWN
        new_direction = 1;
    } else {
        // Right quadrant - RIGHT
        new_direction = 0;
    }
    
    // Only change direction if it's valid (not 180 degrees)
    if (isValidDirectionChange(direction, new_direction)) {
        if (new_direction != direction) {
            direction = new_direction;
            Serial.printf("Direction changed to %d\n", direction);
        } else {
            Serial.println("Direction unchanged");
        }
    } else {
        Serial.println("Invalid direction change - would cause collision");
    }
}

void SnakeGame::joystick_event_cb(lv_event_t *e) {
    SnakeGame *self = static_cast<SnakeGame *>(lv_event_get_user_data(e));
    lv_obj_t *obj = (lv_obj_t*)lv_event_get_target(e);  // Add explicit cast
    int new_direction = (int)lv_obj_get_user_data(obj);
    
    // Only change direction if it's valid (not 180 degrees)
    if (self->isValidDirectionChange(self->direction, new_direction)) {
        self->direction = new_direction;
        Serial.printf("Direction changed to %d via joystick\n", new_direction);
        
        // Visual feedback - highlight the button briefly
        lv_obj_set_style_bg_color(obj, lv_color_hex(0x88AAFF), LV_PART_MAIN);
        lv_timer_t *timer = lv_timer_create([](lv_timer_t *t) {
            lv_obj_set_style_bg_color((lv_obj_t*)t->user_data, lv_color_hex(0x4444FF), LV_PART_MAIN);
            lv_timer_del(t);
        }, 150, obj);
    }
}

// Pause button event callback
void SnakeGame::pause_event_cb(lv_event_t *e) {
    // Empty implementation - button removed
}

void SnakeGame::check_touch_input() {
    if (current_state != STATE_PLAYING) return;
    
    static uint32_t last_touch_check = 0;
    uint32_t now = millis();
    
    // Only check touch every 50ms to avoid excessive polling
    if (now - last_touch_check < 50) return;
    
    last_touch_check = now;
    
    uint16_t x, y;
    if (tft.getTouch(&x, &y)) {
        Serial.printf("Direct touch detected at (%d, %d)\n", x, y);
        
        // Update touch regions for new control layout
        int new_direction = -1;
        
        // Left side UP button region
        if (x >= 20 && x <= 100 && y >= 120 && y <= 200) {
            new_direction = 3; // UP
        }
        // Left side LEFT button region
        else if (x >= 20 && x <= 100 && y >= 220 && y <= 300) {
            new_direction = 2; // LEFT
        }
        // Left side DOWN button region
        else if (x >= 20 && x <= 100 && y >= 320 && y <= 400) {
            new_direction = 1; // DOWN
        }
        // Right side UP button region
        else if (x >= LV_HOR_RES - 100 && x <= LV_HOR_RES - 20 && y >= 120 && y <= 200) {
            new_direction = 3; // UP
        }
        // Right side RIGHT button region
        else if (x >= LV_HOR_RES - 100 && x <= LV_HOR_RES - 20 && y >= 220 && y <= 300) {
            new_direction = 0; // RIGHT
        }
        // Right side DOWN button region
        else if (x >= LV_HOR_RES - 100 && x <= LV_HOR_RES - 20 && y >= 320 && y <= 400) {
            new_direction = 1; // DOWN
        }
        // For other areas, use the existing direction logic for generic screen touches
        else {
            // Simple directional control based on screen quadrants
            if (y < LV_VER_RES/2) {
                new_direction = 3; // UP
            } else {
                new_direction = 1; // DOWN
            }
            
            if (x < LV_HOR_RES/2) {
                new_direction = 2; // LEFT
            } else {
                new_direction = 0; // RIGHT
            }
        }
        
        // Only change direction if it's valid (not 180 degrees)
        if (new_direction >= 0 && isValidDirectionChange(direction, new_direction) && new_direction != direction) {
            direction = new_direction;
            Serial.printf("Direction changed to %d via touch\n", direction);
        }
    }
}

void SnakeGame::show_game_over() {
    current_state = STATE_GAME_OVER;
    
    // Stop game timer
    if (game_timer) {
        lv_timer_del(game_timer);
        game_timer = nullptr;
    }
    
    // Update high score
    if (score > high_score) {
        high_score = score;
    }
    
    // Create semi-transparent overlay
    lv_obj_t *overlay = lv_obj_create(scr);
    if (!overlay) {
        Serial.println("ERROR: Could not create game over overlay");
        return;
    }
    
    lv_obj_set_size(overlay, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_bg_color(overlay, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(overlay, LV_OPA_70, LV_PART_MAIN);
    lv_obj_set_style_border_width(overlay, 0, LV_PART_MAIN);
    
    // Game over text
    lv_obj_t *game_over_label = lv_label_create(overlay);
    if (game_over_label) {
        lv_obj_set_style_text_font(game_over_label, &lv_font_montserrat_30, LV_PART_MAIN);
        lv_obj_set_style_text_color(game_over_label, lv_color_hex(0xFF0000), LV_PART_MAIN);
        lv_label_set_text(game_over_label, "GAME OVER");
        lv_obj_align(game_over_label, LV_ALIGN_CENTER, 0, -50);
    }
    
    // Score text
    lv_obj_t *final_score_label = lv_label_create(overlay);
    if (final_score_label) {
        lv_obj_set_style_text_font(final_score_label, &lv_font_montserrat_24, LV_PART_MAIN);
        lv_obj_set_style_text_color(final_score_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
        lv_label_set_text_fmt(final_score_label, "Your Score: %d", score);
        lv_obj_align(final_score_label, LV_ALIGN_CENTER, 0, 0);
    }
    
    // High score text
    lv_obj_t *high_score_end_label = lv_label_create(overlay);
    if (high_score_end_label) {
        lv_obj_set_style_text_color(high_score_end_label, lv_color_hex(0xFFD700), LV_PART_MAIN);
        lv_label_set_text_fmt(high_score_end_label, "High Score: %d", high_score);
        lv_obj_align(high_score_end_label, LV_ALIGN_CENTER, 0, 40);
    }
    
    // Start timer to return to start screen - slightly longer time
    game_over_timer = lv_timer_create(game_over_timer_cb, 5000, this);
}

void SnakeGame::stop() {
    if (!game_running) {
        Serial.println("Game not running, ignoring stop request");
        return;
    }
    
    Serial.println("Stopping snake game...");
    
    // Set flag first to prevent further game updates
    game_running = false;
    
    // Reset watchdog before cleanup operations
    esp_task_wdt_reset();
    
    // Delete timers with null checks to avoid deleting invalid timers
    if (game_timer) {
        lv_timer_del(game_timer);
        game_timer = nullptr;
    }
    
    if (animation_timer) {
        lv_timer_del(animation_timer);
        animation_timer = nullptr;
    }
    
    if (game_over_timer) {
        lv_timer_del(game_over_timer);
        game_over_timer = nullptr;
    }
    
    // Process any pending LVGL operations before screen restoration
    lv_timer_handler();
    delay(10);
    
    // Return to previous screen and clean up resources
    restore_screen();
    
    // Perform an additional cleanup to ensure all memory is properly freed
    reset();
    
    // Process LVGL tasks after complete cleanup
    lv_timer_handler();
    
    // Reset watchdog one final time
    esp_task_wdt_reset();
    
    Serial.println("Snake game stopped successfully");
}

void SnakeGame::restore_screen() {
    Serial.println("Restoring previous screen");
    
    // Reset watchdog before screen restoration
    esp_task_wdt_reset();
    
    // First ensure all our timers are stopped
    if (game_timer) {
        lv_timer_del(game_timer);
        game_timer = nullptr;
    }
    
    if (animation_timer) {
        lv_timer_del(animation_timer);
        animation_timer = nullptr;
    }
    
    if (game_over_timer) {
        lv_timer_del(game_over_timer);
        game_over_timer = nullptr;
    }
    
    // Ensure we're not running anymore
    game_running = false;
    
    if (!prev_screen) {
        Serial.println("WARNING: No previous screen to restore");
        return;
    }
    
    // Clean up current screen to avoid memory leaks
    lv_obj_t* current_screen = lv_scr_act();
    
    // Make sure we're not in the middle of a delete operation
    if (!current_screen) {
        Serial.println("WARNING: No active screen to restore from");
        return;
    }
    
    // Clear all object references BEFORE we delete anything
    for (int i = 0; i < MAX_SNAKE_LENGTH; i++) {
        snake_body[i] = nullptr;
    }
    for (int i = 0; i < 5; i++) {
        demo_snake[i] = nullptr;
    }
    food = nullptr;
    score_label = nullptr;
    start_container = nullptr;
    title_label = nullptr;
    high_score_label = nullptr;
    for (int i = 0; i < 3; i++) {
        difficulty_buttons[i] = nullptr;
    }
    start_button = nullptr;
    
    // Save previous screen locally and clear the member variable
    lv_obj_t* local_prev_screen = prev_screen;
    scr = nullptr;
    prev_screen = nullptr;
    
    // Wait for any pending LVGL operations to complete
    lv_timer_handler();
    
    // Special handling to avoid crashes during screen transition
    // Perform the screen switch first - do not delete yet
    if (local_prev_screen) {
        // Use a direct screen load without animation
        lv_scr_load(local_prev_screen);
        
        // Force an immediate refresh
        lv_refr_now(NULL);
        
        // Process any pending operations
        lv_timer_handler();
        delay(10);
    }
    
    // Only now delete the snake game screen (if it's not the current screen)
    if (current_screen != local_prev_screen) {
        // Delete the screen after we've switched away from it
        if (current_screen) {
            // Use an immediate delete since we've already switched away
            lv_obj_del(current_screen);
            Serial.println("Deleted snake game screen");
        }
    }
    
    // Reset input devices to clear any pending events
    lv_indev_t* indev = lv_indev_get_next(NULL);
    while (indev) {
        lv_indev_reset(indev, NULL);  // Reset all input devices
        indev = lv_indev_get_next(indev);
    }
    
    // Force a refresh of the screen
    lv_obj_invalidate(lv_scr_act());
    lv_refr_now(NULL);
    
    // Process any pending events before we return
    lv_timer_handler();
    
    // Allow time for screen transition to complete
    delay(50);
    
    // Reset watchdog before returning
    esp_task_wdt_reset();
    
    Serial.println("Screen restored successfully");
}

void SnakeGame::reset() {
    Serial.println("Resetting Snake game state");
    
    // Reset watchdog before cleanup operations
    esp_task_wdt_reset();
    
    // Complete reset of game state
    game_running = false;
    current_state = STATE_START_SCREEN;
    direction = 0;
    snake_length = 3;
    score = 0;
    head_x = 0;
    head_y = 0;
    
    // At this point, we should have already cleared all object pointers
    // in the restore_screen method, but let's be extra safe
    for (int i = 0; i < MAX_SNAKE_LENGTH; i++) {
        snake_body[i] = nullptr;
    }
    for (int i = 0; i < 5; i++) {
        demo_snake[i] = nullptr;
    }
    
    food = nullptr;
    score_label = nullptr;
    start_container = nullptr;
    title_label = nullptr;
    high_score_label = nullptr;
    for (int i = 0; i < 3; i++) {
        difficulty_buttons[i] = nullptr;
    }
    start_button = nullptr;
    
    // These are set to null by restore_screen() but make sure just in case
    scr = nullptr;
    prev_screen = nullptr;

    // Allow time for any pending operations to complete
    delay(10);
    
    // Reset watchdog again after cleanup
    esp_task_wdt_reset();

    Serial.println("Snake game state reset complete");
}
