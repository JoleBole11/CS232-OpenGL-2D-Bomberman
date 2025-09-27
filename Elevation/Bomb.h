#pragma once
#include "GameObject.h"
#include "Input.h"
#include "GameInstance.h"
#include "GameScene.h"
#include "CharacterTypesEnum.h"

// Forward declaration
class GameScene;

class Bomb :
    public GameObject
{
private:
    int radius;
    float timer;
    float initial_timer;
    std::vector<std::vector<int>>* object_map;
    int tile_x, tile_y;
    CharacterType character; // Add character type

public:
    Bomb(const glm::vec2& pos, const glm::vec2& vel, Sprite* spr, float time,
        std::vector<std::vector<int>>* _object_map, int tx, int ty, int rad, CharacterType char_type);
    ~Bomb();


    void update(float dt);
    CharacterType getCharacter() const { return character; }
};