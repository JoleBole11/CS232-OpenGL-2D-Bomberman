#include "Explosion.h"

bool Explosion::wall_type_check(int height_map_index, int y, int x, int i, int frame_edge, int frame_normal)
{
    if (height_map[height_map_index] == 1) { // Empty check
        (*object_map)[y][x] = Object::KILL_OBJECT;
        int frame = (i == static_cast<int>(radius)) ? frame_edge : frame_normal;
        explosion_positions.push_back({ x, y, frame });
        return false;
    }
    else if (height_map[height_map_index] >= 2) { // Wall check
        if ((*tile_map)[12 - y][x] == Wall::BREAKABLE) { // Destructible check
            (*tile_map)[12 - y][x] = 0;
            height_map[height_map_index] = 1;

            (*object_map)[y][x] = Object::KILL_OBJECT;
            int frame = 4;
            explosion_positions.push_back({ x, y, frame_edge }); 
        }
        else if ((*tile_map)[12 - y][x] == Wall::RADIUS) {
            (*tile_map)[12 - y][x] = 0;
            height_map[height_map_index] = 1;
            (*object_map)[y][x] = Object::PICKUP_RADIUS;
        }
        else if ((*tile_map)[12 - y][x] == Wall::SPEED) {
            (*tile_map)[12 - y][x] = 0;
            height_map[height_map_index] = 1;
            (*object_map)[y][x] = Object::PICKUP_SPEED;
        }
        Game::game_instance->set_walls_destroyed(true);
        return true;
    }
}

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

        float origin_x = (960 - cols * tile_size) / 2.0f;
        float origin_y = (832 - rows * tile_size) / 2.0f;

        int center_x = static_cast<int>((get_position().x - origin_x) / tile_size);
        int center_y = static_cast<int>((get_position().y - origin_y) / tile_size);

        std::cout << "Explosion at world pos (" << get_position().x << ", " << get_position().y << ")" << std::endl;
        std::cout << "Calculated tile pos (" << center_x << ", " << center_y << std::endl;

        if (center_x < 0 || center_x >= cols || center_y < 0 || center_y >= rows) {
            std::cout << "Explosion position out of bounds!" << std::endl;
            set_is_active(false);
            set_is_visible(false);
            return;
        }

        (*object_map)[center_y][center_x] = Object::KILL_OBJECT;
        explosion_positions.push_back({ center_x, center_y, 0 });

        // Vertical Up
        for (int i = 1; i <= radius; ++i) {
            int world_y_up = center_y + i;

			if (world_y_up < rows && world_y_up >= 0) { // Bounds check
                int height_map_index = center_x + world_y_up * cols;

				if (wall_type_check(height_map_index, world_y_up, center_x, i, 6, 2)) break;
            }
        }

        // Vertical Down
        for (int i = 1; i <= radius; ++i) {
            int world_y_down = center_y - i;

			if (world_y_down >= 0 && world_y_down < rows) { // Bounds check
                int height_map_index = center_x + world_y_down * cols;

				if (wall_type_check(height_map_index, world_y_down, center_x, i, 5, 2)) break;
            }
        }

        // Horizontal Left
        for (int i = 1; i <= radius; ++i) {
            int x_left = center_x - i;
			if (x_left >= 0) { // Bounds check
                int height_map_index = x_left + center_y * cols;

				if (wall_type_check(height_map_index, center_y, x_left, i, 3, 1)) break;
            }
        }

		// Horizontal Right
        for (int i = 1; i <= radius; ++i) {
            int x_right = center_x + i;
			if (x_right < cols) { // Bounds check
                int height_map_index = x_right + center_y * cols;

				if (wall_type_check(height_map_index, center_y, x_right, i, 4, 1)) break;
            }
        }

        explosion_applied = true;
    }

    if (timer <= 0.0f) {
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

    for (const auto& pos : explosion_positions) {
        glPushMatrix();

        float world_x = pos.tile_x * tile_size + origin_x;
        float world_y = pos.tile_y * tile_size + origin_y;

        glTranslatef(world_x, world_y, 0.0f);

        get_sprite()->set_current_frame(pos.frame);
        get_sprite()->render();

        glPopMatrix();
    }
}