#pragma once
#include "GameObject.h"
#include "Input.h"
#include "MainMenuScene.h"
#include "GameInstance.h"
#include <vector>
#include "GameScene.h"

// Forward declaration
class GameScene;

class Player :
    public GameObject
{
private:
    int bomb_radius = 2;
    int* height_map;
    int player_id;

    std::vector<std::vector<int>>* object_map;

    float bomb_cooldown = 0.0f;
    float speed = 100;
    float radius_timer = 0;
    float speed_timer = 0;

    bool dead = false;
    bool radius_powered = false;
    bool speed_powered = false;

    bool can_move_to_position(const glm::vec2& position);

public:
    Player(glm::vec2 pos, glm::vec2 velocity, Sprite* sprite, int* _height_map, std::vector<std::vector<int>>* object_map, int playerId = 1);
    ~Player();

    float get_speed() const { return speed; };
    void set_speed(float s) { speed = s; };

    int get_bomb_radius() const { return bomb_radius; };
    void set_bomb_radius(int r) { bomb_radius = r; };

    float get_radius_timer() const { return radius_timer; };
    void set_radius_timer(float t) { radius_timer = t; };

    float get_speed_timer() const { return speed_timer; };
    void set_speed_timer(float t) { speed_timer = t; };

    int get_player_id() const { return player_id; };

    void update(float dt);
};