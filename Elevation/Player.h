#pragma once
#include "GameObject.h"
#include "Input.h"
#include "Game.h"

class Player :
    public GameObject
{
private:
    float speed = 100;
    int* height_map;
    std::vector<std::vector<int>>* object_map;
    float bomb_cooldown = 0.0f;  // Add cooldown to prevent spam

    bool can_move_to_position(const glm::vec2& position);

public:
    Player(glm::vec2 pos, glm::vec2 velocity, Sprite* sprite, int* _height_map, std::vector<std::vector<int>>* object_map);

    void update(float dt);
};