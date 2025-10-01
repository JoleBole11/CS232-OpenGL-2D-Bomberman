#include "Player.h"
#include "GameInstance.h"
#include "GameScene.h"
#include "Bomb.h"
#include "AudioManager.h"

Player::Player(glm::vec2 pos, glm::vec2 velocity, Sprite* sprite, int* _height_map, std::vector<std::vector<int>>* _object_map, int playerId) :
    GameObject(pos, velocity, sprite), frame_timer(0.0f), current_anim_frame(0)
{
    height_map = _height_map;
    object_map = _object_map;
    bomb_cooldown = 0.0f;
    player_id = playerId;
}

Player::~Player()
{
}

void Player::Place_wall(int tile_x, int tile_y)
{
    GameScene* gameScene = GameInstance::getCurrentGameScene();
    if (gameScene) {
        gameScene->addWall(tile_x, tile_y);
    }
    wall_timer = 10.0f;
    AudioManager::playSound("placeBomb");
}

void Player::update(float dt)
{
    if (dead) {
        set_is_active(false);
        set_is_visible(false);
        return;
    }

    glm::vec2 center = get_position() + glm::vec2(32, 32);
    int tile_x = static_cast<int>(center.x / 64.0f);
    int tile_y = static_cast<int>(center.y / 64.0f);

    if (bomb_cooldown >= 0.0f)
        bomb_cooldown -= dt;

    if (availabe_bombs < 1 && bomb_cooldown <= 0.0f) {
        availabe_bombs = 1;
	}

    if (wall_timer >= 0.0f)
        wall_timer -= dt;

    if (radius_timer >= 0.0f) {
        radius_timer -= dt;
        if (radius_timer <= 0.0f && radius_powered) {
            radius_powered = false;
            set_bomb_radius(bomb_radius - 2);
            radius_timer = 0.0f;
            AudioManager::playSound("losePickup");
        }
    }
    if (speed_timer >= 0.0f) {
        speed_timer -= dt;
        if (speed_timer <= 0.0f && speed_powered) {
            speed_powered = false;
            set_speed(speed - 50);
            speed_timer = 0.0f;
            AudioManager::playSound("losePickup");
        }
    }

    set_velocity(glm::vec2(0, 0));
    Sprite* sprite = this->get_sprite();
    int current_direction_frame = 0;
    glm::vec2 flip = sprite->get_sprite_flip();
    bool moving = false;

    if (player_id == 1) {
        if (Input::getKey('W')) {
            set_velocity(glm::vec2(0, speed));
            moving = true;
            flip.x = false;
            sprite->set_sprite_flip(flip);
            current_direction_frame = 4;
            if (Input::getKeyDown('C') && wall_timer <= 0.0f && number_of_walls >= 1) {
				Place_wall(tile_x, tile_y + 1);
            }
        }
        else if (Input::getKey('S')) {
            set_velocity(glm::vec2(0, -speed));
            moving = true;
            flip.x = false;
            sprite->set_sprite_flip(flip);
            current_direction_frame = 0;
            if (Input::getKeyDown('C') && wall_timer <= 0.0f && number_of_walls >= 1) {
				Place_wall(tile_x, tile_y - 1);
            }
        }
        else if (Input::getKey('A')) {
            set_velocity(glm::vec2(-speed, 0));
            moving = true;
            flip.x = false;
            sprite->set_sprite_flip(flip);
            current_direction_frame = 8;
            if (Input::getKeyDown('C') && wall_timer <= 0.0f && number_of_walls >= 1) {
				Place_wall(tile_x - 1, tile_y);
            }
        }
        else if (Input::getKey('D')) {
            set_velocity(glm::vec2(speed, 0));
            moving = true;
            flip.x = true;
            sprite->set_sprite_flip(flip);
            current_direction_frame = 8;
            if (Input::getKeyDown('C') && wall_timer <= 0.0f && number_of_walls >= 1) {
				Place_wall(tile_x + 1, tile_y);
            }
        }

        if (Input::getKeyDown('V') && availabe_bombs >= 1) {
            GameScene* gameScene = GameInstance::getCurrentGameScene();
            if (gameScene) {
                gameScene->addBomb(tile_x, tile_y, bomb_radius, player_id);
            }
			availabe_bombs--;
			bomb_cooldown = 3.0f;
            AudioManager::playSound("placeBomb");
        }

        
    }
    else if (player_id == 2) {
        if (Input::getKey('I')) {
            set_velocity(glm::vec2(0, speed));
            moving = true;
            flip.x = false;
            sprite->set_sprite_flip(flip);
            current_direction_frame = 4;
            if (Input::getKeyDown(',') && wall_timer <= 0.0f && number_of_walls >= 1) {
				Place_wall(tile_x, tile_y + 1);
            }
        }
        else if (Input::getKey('K')) {
            set_velocity(glm::vec2(0, -speed));
            moving = true;
            flip.x = false;
            sprite->set_sprite_flip(flip);
            current_direction_frame = 0;
            if (Input::getKeyDown(',') && wall_timer <= 0.0f && number_of_walls >= 1) {
				Place_wall(tile_x, tile_y - 1);
            }
        }
        else if (Input::getKey('J')) {
            set_velocity(glm::vec2(-speed, 0));
            moving = true;
            flip.x = false;
            sprite->set_sprite_flip(flip);
            current_direction_frame = 8;
            if (Input::getKeyDown(',') && wall_timer <= 0.0f && number_of_walls >= 1) {
				Place_wall(tile_x - 1, tile_y);
            }
        }
        else if (Input::getKey('L')) {
            set_velocity(glm::vec2(speed, 0));
            moving = true;
            flip.x = true;
            sprite->set_sprite_flip(flip);
            current_direction_frame = 8;
            if (Input::getKeyDown(',') && wall_timer <= 0.0f && number_of_walls >= 1) {
                Place_wall(tile_x + 1, tile_y);
            }
        }

        if (Input::getKeyDown('.') && bomb_cooldown <= 0.0f) {
            GameScene* gameScene = GameInstance::getCurrentGameScene();
            if (gameScene) {
                gameScene->addBomb(tile_x, tile_y, bomb_radius, player_id);
            }
            bomb_cooldown = 3.0f;
            AudioManager::playSound("placeBomb");
        }
    }

    if (tile_y >= 0 && tile_y < object_map->size() &&
        tile_x >= 0 && tile_x < (*object_map)[tile_y].size()) {

        if ((*object_map)[tile_y][tile_x] == Object::KILL_OBJECT)
            dead = true;

        if ((*object_map)[tile_y][tile_x] == Object::PICKUP_SPEED) {
            set_speed(speed + 50);
            (*object_map)[tile_y][tile_x] = 0;

            GameScene* gameScene = GameInstance::getCurrentGameScene();
            if (gameScene) {
                gameScene->removeObjectAt(tile_x, tile_y);
                gameScene->setWallsDestroyed(true);
            }

            set_speed_timer(5.0f);
            speed_powered = true;
            AudioManager::playSound("gainPickup");
        }
        if ((*object_map)[tile_y][tile_x] == Object::PICKUP_RADIUS) {
            set_bomb_radius(bomb_radius + 2);
            (*object_map)[tile_y][tile_x] = 0;

            GameScene* gameScene = GameInstance::getCurrentGameScene();
            if (gameScene) {
                gameScene->removeObjectAt(tile_x, tile_y);
            }

            set_radius_timer(5.0f);
            radius_powered = true;
            AudioManager::playSound("gainPickup");
        }
        if ((*object_map)[tile_y][tile_x] == Object::PICKUP_BOMB) {
			set_availabe_bombs(availabe_bombs + 1);
            (*object_map)[tile_y][tile_x] = 0;

            GameScene* gameScene = GameInstance::getCurrentGameScene();
            if (gameScene) {
                gameScene->removeObjectAt(tile_x, tile_y);
            }

            AudioManager::playSound("gainPickup");
        }
    }

    const float ANIMATION_SPEED = 0.15f;
    const int FRAMES= 4; 

    if (moving) {
        frame_timer += dt;
        if (frame_timer >= ANIMATION_SPEED) {
            current_anim_frame = (current_anim_frame + 1) % FRAMES;
            frame_timer = 0.0f;
        }
        sprite->set_current_frame(current_direction_frame + current_anim_frame);
    }
    else {
        current_anim_frame = 0;
        frame_timer = 0.0f;
        sprite->set_current_frame(1);
    }

    glm::vec2 current_pos = get_position();
    glm::vec2 x_movement = glm::vec2(get_velocity().x * dt, 0);
    glm::vec2 new_x_position = current_pos + x_movement;

    if (can_move_to_position(new_x_position)) {
        current_pos = new_x_position;
    }

    glm::vec2 y_movement = glm::vec2(0, get_velocity().y * dt);
    glm::vec2 new_y_position = current_pos + y_movement;

    if (can_move_to_position(new_y_position)) {
        current_pos = new_y_position;
    }

    set_position(current_pos);
}

bool Player::can_move_to_position(const glm::vec2& position)
{
    const float margin = 32.0f;

    glm::vec2 corners[4] = {
        position + glm::vec2(margin, margin),                          // Top left
        position + glm::vec2(64 - margin - 1, margin),                // Top right  
        position + glm::vec2(margin, 64 - margin - 1),                // Bottom left
        position + glm::vec2(64 - margin - 1, 64 - margin - 1)       // Bottom right
    };

    for (int i = 0; i < 4; i++) {
        int tile_x = static_cast<int>(corners[i].x / 64.0f);
        int tile_y = static_cast<int>(corners[i].y / 64.0f);

        if (tile_x < 0 || tile_x >= 15 || tile_y < 0 || tile_y >= 13) {
            return false;
        }

        int height_map_index = tile_x + tile_y * 15;

        if (height_map[height_map_index] != 1) {
            return false;
        }
    }

    return true;
}