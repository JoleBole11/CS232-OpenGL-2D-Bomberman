#pragma once
#include "GameObject.h"
#include "GameInstance.h"  // Add this include
#include <vector>

// Forward declaration
class GameScene;

class Explosion : public GameObject
{
private:
    int radius;
    float timer;
    float initial_timer;
    std::vector<std::vector<int>>* object_map;
    std::vector<std::vector<int>>* tile_map;
    int* height_map;
    bool explosion_applied;
    int tile_x, tile_y;

    struct ExplosionTile {
        int tile_x;
        int tile_y;
        int frame;
    };
    std::vector<ExplosionTile> explosion_positions;

    bool wall_type_check(int height_map_index, int y, int x, int i, int frame_edge, int frame_normal);

public:
    Explosion(const glm::vec2& pos, const glm::vec2& vel, Sprite* spr, int rad,
        std::vector<std::vector<int>>* _object_map, std::vector<std::vector<int>>* _tile_map, int tx, int ty, int* _height_map);
    ~Explosion();

    void update(float dt);
    void render() override;
};