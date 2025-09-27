#pragma once
#include "Scene.h"
#include "Player.h"
#include "GameObject.h"
#include "WallsEnum.h"
#include "Input.h"
#include <vector>
#include "CharacterTypesEnum.h"
#include "MainMenuScene.h"
#include "GameInstance.h"

// Forward declarations to avoid circular dependency
class GameInstance;
class MainMenuScene;
class Player;

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

    // Character selection
    CharacterType player1Character;
    CharacterType player2Character;

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
    const char* getCharacterSpritePath(CharacterType character);
    const char* getBombSpritePath(CharacterType character);
    const char* getExplosionSpritePath(CharacterType character);
    void initializePlayersWithCharacters();
    CharacterType getPlayerCharacterById(int playerId);

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

    // Game-specific methods - Updated to include player ID for character-specific colors
    void addBomb(int tile_x, int tile_y, int radius, int playerId);
    void addExplosion(int tile_x, int tile_y, int radius, CharacterType character);
    void removeObjectAt(int tile_x, int tile_y);
    void setWallsDestroyed(bool destroyed) { walls_destroyed = destroyed; }

    // Character selection
    void setPlayerCharacters(CharacterType p1Char, CharacterType p2Char);

    // Getters for game state
    const std::vector<std::vector<int>>& getTileMap() const { return tile_map; }
    const std::vector<std::vector<int>>& getObjectMap() const { return object_map; }
    int* getHeightMap() const { return height_map; }
};