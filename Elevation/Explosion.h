#pragma once
#include "Bomb.h"
class Explosion :
    public Bomb
{
private:
    float radius;
	std::vector<std::vector<int>>* object_map;

public:
	Explosion(const glm::vec2& pos, const glm::vec2& vel, Sprite* spr, float time, float rad, std::vector<std::vector<int>>* object_map);

	void TriggerExplosion();

	void update(float dt);
};

