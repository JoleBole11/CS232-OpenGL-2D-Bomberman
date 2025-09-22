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

public:
	Player(glm::vec2 pos, glm::vec2 velocity, Sprite* sprite, int* _height_map);

	void update(float dt);
};