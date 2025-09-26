#pragma once
#include "Scene.h"
#include "Player.h"
#include "GameObject.h"
#include "WallsEnum.h"
#include "Input.h"
#include <vector>

class GameScene : public Scene {
private:
    // Game objects
    std::vector<Player*> players;
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
    void initializeMaps();

public:
    GameScene();
    ~GameScene() override;

    void initialize() override;
    void update(float deltaTime) override;
    void render() override;
    void cleanup() override;
	void onEnter() override;
	void onExit() override;

    void handleKeyboard(unsigned char key, int x, int y) override;

    // Game-specific methods
    void addBomb(int tile_x, int tile_y, int radius);
    void addExplosion(int tile_x, int tile_y, int radius);
    void removeObjectAt(int tile_x, int tile_y);
    void setWallsDestroyed(bool destroyed) { walls_destroyed = destroyed; }

    // Getters for game state
    const std::vector<std::vector<int>>& getTileMap() const { return tile_map; }
    const std::vector<std::vector<int>>& getObjectMap() const { return object_map; }
    int* getHeightMap() const { return height_map; }
};