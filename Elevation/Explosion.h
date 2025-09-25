#pragma once
#include "GameObject.h"
#include "Game.h"

class Explosion : public GameObject
{
private:
    float radius;
    float timer;
    float initial_timer;
    std::vector<std::vector<int>>* object_map;
    std::vector<std::vector<int>>* tile_map;
    bool explosion_applied;
    int tile_x, tile_y;

public:
    Explosion(const glm::vec2& pos, const glm::vec2& vel, Sprite* spr, float rad,
        std::vector<std::vector<int>>* _object_map, std::vector<std::vector<int>>* _tile_map, int tx, int ty);
    ~Explosion();

    void update(float dt);
};