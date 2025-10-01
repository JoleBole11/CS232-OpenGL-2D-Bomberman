#pragma once
#include "GameObject.h"
#include "Input.h"
#include "MainMenuScene.h"
#include "GameInstance.h"
#include <vector>
#include "GameScene.h"

class GameScene;

class Player :
    public GameObject
{
private:
    int bomb_radius = 2;
	int availabe_bombs = 1;
	int number_of_walls = 3;
    int* height_map;
    int player_id;

    std::vector<std::vector<int>>* object_map;

    float bomb_cooldown = 0.0f;
	float wall_timer = 0.0f;
    float speed = 100;
    float radius_timer = 0;
    float speed_timer = 0;

    bool dead = false;
    bool radius_powered = false;
    bool speed_powered = false;

    float frame_timer = 0.0f;
    int current_anim_frame = 0;

    bool can_move_to_position(const glm::vec2& position);

public:
    Player(glm::vec2 pos, glm::vec2 velocity, Sprite* sprite, int* _height_map, std::vector<std::vector<int>>* object_map, int playerId = 1);
    ~Player();

    float get_speed() const { return speed; };
    void set_speed(float s) { speed = s; };

    int get_bomb_radius() const { return bomb_radius; };
    void set_bomb_radius(int r) { bomb_radius = r; };

	float get_bomb_cooldown() const { return bomb_cooldown; };
	void set_bomb_cooldown(float c) { bomb_cooldown = c; };

	int get_number_of_walls() const { return number_of_walls; };
	void set_number_of_walls(int n) { number_of_walls = n; };

	float get_wall_timer() const { return wall_timer; };
	void set_wall_timer(float t) { wall_timer = t; };

    float get_radius_timer() const { return radius_timer; };
    void set_radius_timer(float t) { radius_timer = t; };

    float get_speed_timer() const { return speed_timer; };
    void set_speed_timer(float t) { speed_timer = t; };

	int get_availabe_bombs() const { return availabe_bombs; };
	void set_availabe_bombs(int b) { availabe_bombs = b; };
    
	bool get_radius_powered() const { return radius_powered; };
	bool get_speed_powered() const { return speed_powered; };

    int get_player_id() const { return player_id; };

    void Place_wall(int tile_x, int tile_y);

    void update(float dt);
};