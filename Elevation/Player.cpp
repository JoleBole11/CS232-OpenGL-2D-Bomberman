#include "Player.h"
#include "GameInstance.h"
#include "GameScene.h"
#include "Bomb.h"

Player::Player(glm::vec2 pos, glm::vec2 velocity, Sprite* sprite, int* _height_map, std::vector<std::vector<int>>* _object_map, int playerId) :
    GameObject(pos, velocity, sprite), frame_timer(0.0f), current_anim_frame(0)  // Initialize animation variables
{
    height_map = _height_map;
    object_map = _object_map;
    bomb_cooldown = 0.0f;
    player_id = playerId;
}

Player::~Player()
{
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

    if (bomb_cooldown >= 0.0f) {
        bomb_cooldown -= dt;
    }
    if (radius_timer >= 0.0f) {
        radius_timer -= dt;
        if (radius_timer <= 0.0f && radius_powered) {
            radius_powered = false;
            set_bomb_radius(bomb_radius - 2);
            radius_timer = 0.0f;
        }
    }
    if (speed_timer >= 0.0f) {
        speed_timer -= dt;
        if (speed_timer <= 0.0f && speed_powered) {
            speed_powered = false;
            set_speed(speed - 50);
            speed_timer = 0.0f;
        }
    }

    set_velocity(glm::vec2(0, 0));
    Sprite* sprite = this->get_sprite();
    int current_direction_frame = 0;
    glm::vec2 flip = sprite->get_sprite_flip();
    bool moving = false;

    // Handle different controls based on player ID
    if (player_id == 1) {
        // Player 1 controls: WASD + V
        if (Input::getKey('W')) {
            set_velocity(glm::vec2(0, speed));
            moving = true;
            flip.x = false;
            sprite->set_sprite_flip(flip);
            current_direction_frame = 4;  // Up animation frames start at 4
        }
        else if (Input::getKey('S')) {
            set_velocity(glm::vec2(0, -speed));
            moving = true;
            flip.x = false;
            sprite->set_sprite_flip(flip);
            current_direction_frame = 0;  // Down animation frames start at 0
        }
        else if (Input::getKey('A')) {
            set_velocity(glm::vec2(-speed, 0));
            moving = true;
            flip.x = false;
            sprite->set_sprite_flip(flip);
            current_direction_frame = 8;  // Left animation frames start at 8
        }
        else if (Input::getKey('D')) {
            set_velocity(glm::vec2(speed, 0));
            moving = true;
            flip.x = true;
            sprite->set_sprite_flip(flip);
            current_direction_frame = 8;  // Right animation frames start at 8
        }

        if (Input::getKeyDown('V') && bomb_cooldown <= 0.0f) {
            GameScene* gameScene = GameInstance::getCurrentGameScene();
            if (gameScene) {
                gameScene->addBomb(tile_x, tile_y, bomb_radius);
            }
            bomb_cooldown = 3.0f;
        }
    }
    else if (player_id == 2) {
        // Player 2 controls: IJKL + N
        if (Input::getKey('I')) { // Up
            set_velocity(glm::vec2(0, speed));
            moving = true;
            flip.x = false;
            sprite->set_sprite_flip(flip);
            current_direction_frame = 4;
        }
        else if (Input::getKey('K')) { // Down
            set_velocity(glm::vec2(0, -speed));
            moving = true;
            flip.x = false;
            sprite->set_sprite_flip(flip);
            current_direction_frame = 0;
        }
        else if (Input::getKey('J')) { // Left
            set_velocity(glm::vec2(-speed, 0));
            moving = true;
            flip.x = false;
            sprite->set_sprite_flip(flip);
            current_direction_frame = 8;
        }
        else if (Input::getKey('L')) { // Right
            set_velocity(glm::vec2(speed, 0));
            moving = true;
            flip.x = true;
            sprite->set_sprite_flip(flip);
            current_direction_frame = 8;
        }

        if (Input::getKeyDown('N') && bomb_cooldown <= 0.0f) {
            GameScene* gameScene = GameInstance::getCurrentGameScene();
            if (gameScene) {
                gameScene->addBomb(tile_x, tile_y, bomb_radius);
            }
            bomb_cooldown = 3.0f;
        }
    }

    // Handle object interactions
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
        }
    }

    // Handle animation
    const float ANIMATION_SPEED = 0.15f;  // Time between frames
    const int FRAMES_PER_DIRECTION = 4;   // Number of animation frames per direction

    if (moving) {
        frame_timer += dt;
        if (frame_timer >= ANIMATION_SPEED) {
            current_anim_frame = (current_anim_frame + 1) % FRAMES_PER_DIRECTION;
            frame_timer = 0.0f;
        }
        // Set the actual sprite frame: base direction frame + animation offset
        sprite->set_current_frame(current_direction_frame + current_anim_frame);
    }
    else {
        // When not moving, show standing frame (frame 1 in each direction)
        current_anim_frame = 0;
        frame_timer = 0.0f;
        sprite->set_current_frame(1);  // Standing frame
    }

    // Handle movement with collision detection
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
    const float margin = 8.0f;

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