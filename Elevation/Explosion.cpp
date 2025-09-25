#include "Explosion.h"

Explosion::Explosion(const glm::vec2& pos, const glm::vec2& vel, Sprite* spr, int rad,
    std::vector<std::vector<int>>* _object_map, std::vector<std::vector<int>>* _tile_map, int tx, int ty, int* _height_map)
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
    height_map = _height_map;

    // Store all explosion positions for rendering
    explosion_positions.clear();
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

        // Set center explosion
        (*object_map)[center_y][center_x] = 2;  // Use 2 for explosion
        explosion_positions.push_back({ center_x, center_y, 6 });  // frame 6 for center

        // Vertical explosion
        for (int i = 1; i <= static_cast<int>(radius); ++i) {
            // Up direction
            int y_up = center_y + i;
            if (y_up < rows) {
                (*object_map)[y_up][center_x] = 2;  // Use 2 for explosion
                int frame = (i == static_cast<int>(radius)) ? 0 : 4;  // top or vertical
                explosion_positions.push_back({ center_x, y_up, frame });
            }

            // Down direction
            int y_down = center_y - i;
            if (y_down >= 0) {
                (*object_map)[y_down][center_x] = 2;  // Use 2 for explosion
                int frame = (i == static_cast<int>(radius)) ? 1 : 4;  // bottom or vertical
                explosion_positions.push_back({ center_x, y_down, frame });
            }
        }

        // Horizontal explosion
        for (int i = 1; i <= static_cast<int>(radius); ++i) {
            // Left direction
            int x_left = center_x - i;
            if (x_left >= 0) {
                (*object_map)[center_y][x_left] = 2;  // Use 2 for explosion
                int frame = (i == static_cast<int>(radius)) ? 2 : 5;  // left or horizontal
                explosion_positions.push_back({ x_left, center_y, frame });
            }

            // Right direction
            int x_right = center_x + i;
            if (x_right < cols) {
                (*object_map)[center_y][x_right] = 2;  // Use 2 for explosion
                int frame = (i == static_cast<int>(radius)) ? 3 : 5;  // right or horizontal
                explosion_positions.push_back({ x_right, center_y, frame });
            }
        }

        explosion_applied = true;
    }

    // Remove explosion after timer expires
    if (timer <= 0.0f) {
        // Clear all explosion positions from object_map
        for (const auto& pos : explosion_positions) {
            (*object_map)[pos.tile_y][pos.tile_x] = 0;
        }
        explosion_positions.clear();

        set_is_active(false);
        set_is_visible(false);
    }
}

void Explosion::render()
{
    if (!get_is_visible() || explosion_positions.empty()) return;

    const int tile_size = 64;
    const int cols = 15;
    const int rows = 13;
    float origin_x = (960 - cols * tile_size) / 2.0f;
    float origin_y = (832 - rows * tile_size) / 2.0f;

    // Render sprite at each explosion position with appropriate frame
    for (const auto& pos : explosion_positions) {
        glPushMatrix();

        // Calculate world position for this tile
        float world_x = pos.tile_x * tile_size + origin_x;
        float world_y = pos.tile_y * tile_size + origin_y;

        glTranslatef(world_x, world_y, 0.0f);

        // Set the appropriate frame and render
        get_sprite()->set_current_frame(pos.frame);
        get_sprite()->render();

        glPopMatrix();
    }
}