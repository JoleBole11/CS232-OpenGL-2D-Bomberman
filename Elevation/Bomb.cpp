#include "Bomb.h"

Bomb::Bomb(const glm::vec2& pos, const glm::vec2& vel, Sprite* spr, float time, std::vector<std::vector<int>>* _object_map, int* _height_map, int tx, int ty)
    : GameObject(pos, vel, spr) 
{
    timer = time;
	initial_timer = time;
	height_map = _height_map;
	object_map = _object_map;
	tile_x = tx;
	tile_y = ty;
}

void Bomb::update(float dt)
{
    Sprite* sprite = this->get_sprite();
    timer -= dt;

    if (timer <= initial_timer / 3.0f)
    {
        sprite->set_current_frame(2);
    }
    else if (timer <= initial_timer * 2.0f / 3.0f)
    {
        sprite->set_current_frame(1);
    }
    else if (timer > 0.0f)
    {
        sprite->set_current_frame(0);
    }

    if (timer <= 0.0f)
    {
        // Clean up object_map and height_map when bomb is destroyed
        if (object_map && tile_y >= 0 && tile_y < object_map->size() &&
            tile_x >= 0 && tile_x < (*object_map)[tile_y].size()) {
            (*object_map)[tile_y][tile_x] = 0;  // tile_y is already array coordinate
        }

        if (height_map && tile_x >= 0 && tile_y >= 0) {
            // Convert array coordinate back to world coordinate for height_map
            int world_y = 12 - tile_y;  // Convert array coord back to world coord
            int height_map_index = tile_x + world_y * 15;
            if (height_map_index >= 0 && height_map_index < 195) {  // 15*13=195
                height_map[height_map_index] = 1;  // Set back to walkable
            }
        }

        set_is_active(false);
        set_is_visible(false);
    }
}