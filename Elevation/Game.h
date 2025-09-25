#pragma once
#include <iostream>
#include <vector>
#include <time.h>
#include "GameObject.h"
#include "Player.h"
#include "WallsEnum.h"

class Game
{
private:
    // Dimensions
    const int width;
    const int height;

    // Map textures
    int texture_cols = 15;
    int texture_size = 13;

    // Height change per tile
    int height_change = 16;

    // Map size
    const int map_size_x = 15;
    const int map_size_y = 13;

    // Screen position
    glm::vec2 screen = glm::vec2(
        width - (15 * texture_size / 2),
        height / 2 - (13 * texture_size / 6)
    );

    // Object textures
    int object_texture_cols = 2;
    int object_texture_size = 128;

    // Maps
    std::vector<std::vector<int>> tile_map;
    int* height_map;
    std::vector<std::vector<int>> object_map;

    //objects
    std::vector<GameObject*> players;
    std::vector<GameObject*> mapV;
    std::vector<GameObject*> tiles;
    std::vector<GameObject*> objects;

    int previous_time;
    float delta_time = 0.0f;
    bool walls_destroyed = false;

    // Functions
    bool load_textures();
    void render();
    void update(float delta_time);
    void init_game();
    void cleanup();
    void reshape(const GLsizei width, const GLsizei height);
    void init_glut();
    void game_loop();
    void callback_functions();

public:

    static Game* game_instance;

    void addBomb(int tile_x, int tile_y);
    void addExplosion(int tile_x, int tile_y);
    void rebuild_tiles();
    void set_walls_destroyed(bool destroyed) { walls_destroyed = destroyed; }

    Game(int argc, char** argv,
        const char* title,
        const int WIDTH, const int HEIGHT,
        const int POSITION_X, const int POSITION_Y);
    virtual ~Game();
};