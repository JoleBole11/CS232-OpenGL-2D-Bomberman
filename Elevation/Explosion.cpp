#include "Explosion.h"
#include "Game.h"

Explosion::Explosion(const glm::vec2& pos, const glm::vec2& vel, Sprite* spr, float rad,
    std::vector<std::vector<int>>* _object_map, std::vector<std::vector<int>>* _tile_map)
    : GameObject(pos, vel, spr)  // Properly call parent constructor
{
    radius = rad;
    timer = 2.0f;  // 2 second lifetime
    initial_timer = 2.0f;
    object_map = _object_map;
    tile_map = _tile_map;
    explosion_applied = false;
}

void Explosion::update(float dt)
{
    timer -= dt;

    // Apply explosion pattern only once at the beginning
    if (!explosion_applied) {
        int tile_size = 64;
        int cols = 15;
        int rows = 13;

        // Get the tile coordinates from world position
        int center_x = static_cast<int>(get_position().x / tile_size);
        int center_y = static_cast<int>(get_position().y / tile_size);

        // Convert to array coordinates (flip Y axis)
        int array_center_y = 12 - center_y;

        // Explosion sprite frame indices
        const int frame_center = 6;
        const int frame_top = 0;
        const int frame_bottom = 1;
        const int frame_left = 2;
        const int frame_right = 3;
        const int frame_vertical = 4;
        const int frame_horizontal = 5;

        // Set center explosion
        if (center_x >= 0 && center_x < cols && array_center_y >= 0 && array_center_y < rows) {
            (*object_map)[array_center_y][center_x] = frame_center;
        }

        // Vertical explosion (up and down)
        for (int i = 1; i <= static_cast<int>(radius); ++i) {
            // Up direction (negative Y in world, positive in array)
            int array_y_up = array_center_y + i;
            if (array_y_up < rows && center_x >= 0 && center_x < cols) {
                if (i == static_cast<int>(radius)) {
                    (*object_map)[array_y_up][center_x] = frame_top;
                }
                else {
                    (*object_map)[array_y_up][center_x] = frame_vertical;
                }
            }

            // Down direction (positive Y in world, negative in array)
            int array_y_down = array_center_y - i;
            if (array_y_down >= 0 && center_x >= 0 && center_x < cols) {
                if (i == static_cast<int>(radius)) {
                    (*object_map)[array_y_down][center_x] = frame_bottom;
                }
                else {
                    (*object_map)[array_y_down][center_x] = frame_vertical;
                }
            }
        }

        // Horizontal explosion (left and right)
        for (int i = 1; i <= static_cast<int>(radius); ++i) {
            // Left direction
            int x_left = center_x - i;
            if (x_left >= 0 && array_center_y >= 0 && array_center_y < rows) {
                if (i == static_cast<int>(radius)) {
                    (*object_map)[array_center_y][x_left] = frame_left;
                }
                else {
                    (*object_map)[array_center_y][x_left] = frame_horizontal;
                }
            }

            // Right direction
            int x_right = center_x + i;
            if (x_right < cols && array_center_y >= 0 && array_center_y < rows) {
                if (i == static_cast<int>(radius)) {
                    (*object_map)[array_center_y][x_right] = frame_right;
                }
                else {
                    (*object_map)[array_center_y][x_right] = frame_horizontal;
                }
            }
        }

        explosion_applied = true;
    }

    // Remove explosion after timer expires
    if (timer <= 0.0f) {
        // Clear explosion from object_map
        int tile_size = 64;
        int cols = 15;
        int rows = 13;

        int center_x = static_cast<int>(get_position().x / tile_size);
        int center_y = static_cast<int>(get_position().y / tile_size);
        int array_center_y = 12 - center_y;

        // Clear center
        if (center_x >= 0 && center_x < cols && array_center_y >= 0 && array_center_y < rows) {
            (*object_map)[array_center_y][center_x] = 0;
        }

        // Clear vertical explosion
        for (int i = 1; i <= static_cast<int>(radius); ++i) {
            int array_y_up = array_center_y + i;
            int array_y_down = array_center_y - i;

            if (array_y_up < rows && center_x >= 0 && center_x < cols) {
                (*object_map)[array_y_up][center_x] = 0;
            }
            if (array_y_down >= 0 && center_x >= 0 && center_x < cols) {
                (*object_map)[array_y_down][center_x] = 0;
            }
        }

        // Clear horizontal explosion
        for (int i = 1; i <= static_cast<int>(radius); ++i) {
            int x_left = center_x - i;
            int x_right = center_x + i;

            if (x_left >= 0 && array_center_y >= 0 && array_center_y < rows) {
                (*object_map)[array_center_y][x_left] = 0;
            }
            if (x_right < cols && array_center_y >= 0 && array_center_y < rows) {
                (*object_map)[array_center_y][x_right] = 0;
            }
        }

        set_is_active(false);
        set_is_visible(false);
    }
}