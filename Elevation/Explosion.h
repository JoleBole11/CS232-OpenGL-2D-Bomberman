#pragma once
#include "GameObject.h"
#include "Game.h"

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

public:
    Explosion(const glm::vec2& pos, const glm::vec2& vel, Sprite* spr, int rad,
        std::vector<std::vector<int>>* _object_map, std::vector<std::vector<int>>* _tile_map, int tx, int ty, int* _height_map);
    ~Explosion();

    void update(float dt);
};