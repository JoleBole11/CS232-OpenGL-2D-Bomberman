#include "Explosion.h"
#include "GameScene.h"
#include "GameInstance.h"
#include <iostream>

bool Explosion::wall_type_check(int height_map_index, int y, int x, int i, int frame_edge, int frame_normal)
{
    if (height_map_index < 0 || height_map_index >= (15 * 13)) {
        return true;
    }

    if (y < 0 || y >= static_cast<int>(object_map->size()) ||
        x < 0 || x >= static_cast<int>((*object_map)[y].size())) {
        return true;
    }

    if (height_map[height_map_index] == 1) { // Empty check
        if ((*object_map)[y][x] == Object::KILL_OBJECT) {
            int frame = (i == static_cast<int>(radius)) ? frame_edge : frame_normal;
            explosion_positions.push_back({ x, y, frame });
            return false;
        }

        (*object_map)[y][x] = Object::KILL_OBJECT;
        int frame = (i == static_cast<int>(radius)) ? frame_edge : frame_normal;
        explosion_positions.push_back({ x, y, frame });
        return false;
    }
    else if (height_map[height_map_index] >= 2) { // Wall check
        int tile_map_row = 12 - y;
        if (tile_map_row < 0 || tile_map_row >= static_cast<int>(tile_map->size()) ||
            x < 0 || x >= static_cast<int>((*tile_map)[tile_map_row].size())) {
            return true;
        }

        if ((*tile_map)[tile_map_row][x] == Wall::BREAKABLE) { // Destructible check
            if ((*tile_map)[tile_map_row][x] == Wall::BREAKABLE) {
                (*tile_map)[tile_map_row][x] = 0;
                height_map[height_map_index] = 1;

                (*object_map)[y][x] = Object::KILL_OBJECT;
                explosion_positions.push_back({ x, y, frame_edge });
            }
        }
        else if ((*tile_map)[tile_map_row][x] == Wall::RADIUS) {
            if ((*tile_map)[tile_map_row][x] == Wall::RADIUS) {
                (*tile_map)[tile_map_row][x] = 0;
                height_map[height_map_index] = 1;
                (*object_map)[y][x] = Object::PICKUP_RADIUS;
            }
        }
        else if ((*tile_map)[tile_map_row][x] == Wall::SPEED) {
            if ((*tile_map)[tile_map_row][x] == Wall::SPEED) {
                (*tile_map)[tile_map_row][x] = 0;
                height_map[height_map_index] = 1;
                (*object_map)[y][x] = Object::PICKUP_SPEED;
            }
        }

        GameScene* gameScene = GameInstance::getCurrentGameScene();
        if (gameScene) {
            gameScene->setWallsDestroyed(true);
        }

        return true;
    }
    return false;
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
    if (!get_is_active()) {
        return;
    }

    timer -= dt;

    if (!explosion_applied) {
        const int tile_size = 64;
        const int cols = 15;
        const int rows = 13;

        float origin_x = (960 - cols * tile_size) / 2.0f;
        float origin_y = (832 - rows * tile_size) / 2.0f;

        int center_x = static_cast<int>((get_position().x - origin_x) / tile_size);
        int center_y = static_cast<int>((get_position().y - origin_y) / tile_size);


        if (center_x < 0 || center_x >= cols || center_y < 0 || center_y >= rows) {
            std::cout << "Explosion position out of bounds" << std::endl;
            set_is_active(false);
            set_is_visible(false);
            explosion_applied = true;
            return;
        }

        if (center_y >= 0 && center_y < static_cast<int>(object_map->size()) &&
            center_x >= 0 && center_x < static_cast<int>((*object_map)[center_y].size())) {

            if ((*object_map)[center_y][center_x] != Object::KILL_OBJECT) {
                (*object_map)[center_y][center_x] = Object::KILL_OBJECT;
            }
            explosion_positions.push_back({ center_x, center_y, 0 });
        }
        applyExplosionDirection(center_x, center_y, cols, rows, 0, 1, 6, 2);   // Up
        applyExplosionDirection(center_x, center_y, cols, rows, 0, -1, 5, 2);  // Down
        applyExplosionDirection(center_x, center_y, cols, rows, -1, 0, 3, 1);  // Left
        applyExplosionDirection(center_x, center_y, cols, rows, 1, 0, 4, 1);   // Right

        explosion_applied = true;
    }

    if (timer <= 0.0f) {
        cleanupExplosion();
        set_is_active(false);
        set_is_visible(false);
    }
}

void Explosion::applyExplosionDirection(int center_x, int center_y, int cols, int rows,
    int dx, int dy, int frame_edge, int frame_normal) {
    for (int i = 1; i <= radius; ++i) {
        int target_x = center_x + (dx * i);
        int target_y = center_y + (dy * i);

        if (target_x < 0 || target_x >= cols || target_y < 0 || target_y >= rows) {
            break;
        }

        int height_map_index = target_x + target_y * cols;

        if (wall_type_check(height_map_index, target_y, target_x, i, frame_edge, frame_normal)) {
            break;
        }
    }
}

void Explosion::cleanupExplosion() {
    if (!object_map) return;

    for (const auto& pos : explosion_positions) {
        if (pos.tile_y >= 0 && pos.tile_y < static_cast<int>(object_map->size()) &&
            pos.tile_x >= 0 && pos.tile_x < static_cast<int>((*object_map)[pos.tile_y].size())) {

            if ((*object_map)[pos.tile_y][pos.tile_x] == Object::KILL_OBJECT) {
                (*object_map)[pos.tile_y][pos.tile_x] = 0;
            }
        }
    }
    explosion_positions.clear();
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
        if (pos.tile_x < 0 || pos.tile_x >= cols || pos.tile_y < 0 || pos.tile_y >= rows) {
            continue;
        }

        glPushMatrix();

        float world_x = pos.tile_x * tile_size + origin_x;
        float world_y = pos.tile_y * tile_size + origin_y;

        glTranslatef(world_x, world_y, 0.0f);

        get_sprite()->set_current_frame(pos.frame);
        get_sprite()->render();

        glPopMatrix();
    }
}