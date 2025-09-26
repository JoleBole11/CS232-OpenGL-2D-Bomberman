#pragma once
#include "Scene.h"
#include "Player.h"
#include "GameObject.h"
#include "WallsEnum.h"
#include <vector>

class Game : public Scene {
private:
    // Game objects
    std::vector<GameObject*> players;
    std::vector<GameObject*> mapV;
    std::vector<GameObject*> tiles;
    std::vector<GameObject*> objects;

    // Maps
    std::vector<std::vector<int>> tile_map;
    int* height_map;
    std::vector<std::vector<int>> object_map;

    // Game state
    bool walls_destroyed;

    // Constants
    const int width = 960;
    const int height = 832;
    const float tile_size = 64.0f;

    // Helper functions
    bool loadTextures();
    void rebuildTiles();
    void updateObjects(float deltaTime);
    void cleanupInactiveObjects();

public:
    Game();
    ~Game() override;

    void initialize() override;
    void update(float deltaTime) override;
    void render() override;
    void cleanup() override;

    void handleKeyboard(unsigned char key, int x, int y) override;

    // Game-specific methods
    void addBomb(int tile_x, int tile_y, int radius);
    void addExplosion(int tile_x, int tile_y, int radius);
    void removeObjectAt(int tile_x, int tile_y);
    void setWallsDestroyed(bool destroyed) { walls_destroyed = destroyed; }
};