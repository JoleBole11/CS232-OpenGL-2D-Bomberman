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

public:
    Bomb(const glm::vec2& pos, const glm::vec2& vel, Sprite* spr, float time);

    void update(float dt);
};