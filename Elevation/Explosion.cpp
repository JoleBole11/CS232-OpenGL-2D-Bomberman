#include "Explosion.h"


Explosion::Explosion(const glm::vec2& pos, const glm::vec2& vel, Sprite* spr, int rad,
    std::vector<std::vector<int>>* _object_map, std::vector<std::vector<int>>* _tile_map, int tx, int ty)
    : GameObject(pos, vel, spr)
{
    radius = rad;
    timer = 2.0f;
    initial_timer = 2.0f;
    object_map = _object_map;
    tile_map = _tile_map;
    explosion_applied = false;
    tile_x = tx;
    tile_y = ty;
}

Explosion::~Explosion()
{
}

void Explosion::update(float dt)
{
    timer -= dt;

    if (!explosion_applied) {
        const int tile_size = 64;
        const int cols = 15;
        const int rows = 13;

        // Calculate center position from world coordinates
        float origin_x = (960 - cols * tile_size) / 2.0f;
        float origin_y = (832 - rows * tile_size) / 2.0f;

        int center_x = static_cast<int>((get_position().x - origin_x) / tile_size);
        int center_y = static_cast<int>((get_position().y - origin_y) / tile_size);

        std::cout << "Explosion at world pos (" << get_position().x << ", " << get_position().y << ")" << std::endl;
        std::cout << "Calculated tile pos (" << center_x << ", " << center_y << ")" << std::endl;

        // Bounds check for center position
        if (center_x < 0 || center_x >= cols || center_y < 0 || center_y >= rows) {
            std::cout << "Explosion position out of bounds!" << std::endl;
            set_is_active(false);
            set_is_visible(false);
            return;
        }

        // Explosion sprite frame indices
        const int frame_center = 6;
        const int frame_top = 0;
        const int frame_bottom = 1;
        const int frame_left = 2;
        const int frame_right = 3;
        const int frame_vertical = 4;
        const int frame_horizontal = 5;

        // Set center explosion
        (*object_map)[center_y][center_x] = frame_center;
        std::cout << "Set center at array[" << center_y << "][" << center_x << "] = " << frame_center << std::endl;

        // Vertical explosion
        for (int i = 1; i <= radius; ++i) {
            // Up direction (positive Y)
            int y_up = center_y + i;
            if (y_up < rows) {
                if (i == static_cast<int>(radius)) {
                    (*object_map)[y_up][center_x] = frame_top;
                }
                else {
                    (*object_map)[y_up][center_x] = frame_vertical;
                }
                std::cout << "Set up explosion at array[" << y_up << "][" << center_x << "]" << std::endl;
            }

            // Down direction (negative Y)
            int y_down = center_y - i;
            if (y_down >= 0) {
                if (i == radius) {
                    (*object_map)[y_down][center_x] = frame_bottom;
                }
                else {
                    (*object_map)[y_down][center_x] = frame_vertical;
                }
                std::cout << "Set down explosion at array[" << y_down << "][" << center_x << "]" << std::endl;
            }
        }

        // Horizontal explosion
        for (int i = 1; i <= radius; ++i) {
            // Left direction
            int x_left = center_x - i;
            if (x_left >= 0) {
                if (i == radius) {
                    (*object_map)[center_y][x_left] = frame_left;
                }
                else {
                    (*object_map)[center_y][x_left] = frame_horizontal;
                }
                std::cout << "Set left explosion at array[" << center_y << "][" << x_left << "]" << std::endl;
            }

            // Right direction
            int x_right = center_x + i;
            if (x_right < cols) {
                if (i == radius) {
                    (*object_map)[center_y][x_right] = frame_right;
                }
                else {
                    (*object_map)[center_y][x_right] = frame_horizontal;
                }
                std::cout << "Set right explosion at array[" << center_y << "][" << x_right << "]" << std::endl;
            }
        }

        explosion_applied = true;
    }

    // Remove explosion after timer expires
    if (timer <= 0.0f) {
        const int tile_size = 64;
        const int cols = 15;
        const int rows = 13;

        float origin_x = (960 - cols * tile_size) / 2.0f;
        float origin_y = (832 - rows * tile_size) / 2.0f;

        int center_x = static_cast<int>((get_position().x - origin_x) / tile_size);
        int center_y = static_cast<int>((get_position().y - origin_y) / tile_size);

        // Clear explosion pattern with bounds checking
        if (center_x >= 0 && center_x < cols && center_y >= 0 && center_y < rows) {
            (*object_map)[center_y][center_x] = 0;

            // Clear vertical
            for (int i = 1; i <= radius; ++i) {
                if (center_y + i < rows) {
                    (*object_map)[center_y + i][center_x] = 0;
                }
                if (center_y - i >= 0) {
                    (*object_map)[center_y - i][center_x] = 0;
                }
            }

            // Clear horizontal
            for (int i = 1; i <= radius; ++i) {
                if (center_x - i >= 0) {
                    (*object_map)[center_y][center_x - i] = 0;
                }
                if (center_x + i < cols) {
                    (*object_map)[center_y][center_x + i] = 0;
                }
            }
        }

        set_is_active(false);
        set_is_visible(false);
    }
}