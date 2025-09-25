#pragma once
#include "GameObject.h"
#include "Game.h"

class Explosion : public GameObject
{
private:
    float radius;
	std::vector<std::vector<int>>* object_map;
	std::vector<std::vector<int>>* tile_map;

public:
	Explosion(const glm::vec2& pos, const glm::vec2& vel, Sprite* spr, float rad, std::vector<std::vector<int>>* _object_map, std::vector<std::vector<int>>* _tile_map);

	void update(float dt);
};

