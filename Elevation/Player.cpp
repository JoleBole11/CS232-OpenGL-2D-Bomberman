#include "Player.h"


Player::Player(glm::vec2 pos, glm::vec2 velocity, Sprite* sprite, int* _height_map) :
	GameObject(pos, velocity, sprite)
{
	height_map = _height_map;
}

void Player::update(float dt)
{
	set_velocity(glm::vec2(0, 0));

    if (Input::getKey('W')) {
		set_velocity(glm::vec2(0, speed));
    }
    if (Input::getKey('S')) {
        set_velocity(glm::vec2(0, -speed));
    }
    if (Input::getKey('A')) {
        set_velocity(glm::vec2(-speed, 0));
    }
    if (Input::getKey('D')) {
        set_velocity(glm::vec2(speed, 0));
    }

	glm::vec2 pos = glm::vec2(get_position().x + 32, get_position().y + 32);
	glm::vec2 new_position = pos + get_velocity() * dt;

    if (height_map[int(new_position.x) / 64 + (13 - int(new_position.y / 64)) * 13] == 1)
    {
        set_position(get_position() + get_velocity() * dt);
    }
    std::cout << "Position: (" << int(new_position.x / 64) << ", " << 13 - int(new_position.y / 64) << ", " << height_map[int(new_position.x / 64 + (13 - (new_position.y / 64))) * 13] << ")\n";
}