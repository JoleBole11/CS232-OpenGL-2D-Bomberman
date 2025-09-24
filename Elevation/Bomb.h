#pragma once
#include "GameObject.h"
#include "Input.h"
#include "Game.h"

class Bomb :
    public GameObject
{
private:
    float timer;
    float initial_timer;
    std::vector<std::vector<int>>* object_map;
    int* height_map;                          
    int tile_x, tile_y;                       

public:
    Bomb(const glm::vec2& pos, const glm::vec2& vel, Sprite* spr, float time,
        std::vector<std::vector<int>>* _object_map, int* _height_map, int tx, int ty);

    void update(float dt);
};