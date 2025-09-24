#include "Bomb.h"

Bomb::Bomb(const glm::vec2& pos, const glm::vec2& vel, Sprite* spr, float time)
    : GameObject(pos, vel, spr) 
{
    timer = time;
	initial_timer = time;
}

void Bomb::update(float dt) 
{
    Sprite* sprite = this->get_sprite();
    timer -= dt;

    if (timer <= initial_timer / 3.0f)
    {
        sprite->set_current_frame(2);
    }
    else if (timer <= initial_timer * 2.0f / 3.0f)
    {
        sprite->set_current_frame(1);
    }
    else if (timer > 0.0f)
    {
        sprite->set_current_frame(0);
    }

    if (timer <= 0.0f)
    {
        set_is_active(false);
        set_is_visible(false);
    }
}