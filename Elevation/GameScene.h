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
#include "WinScene.h"
#include "TextRenderer.h"

class GameInstance;
class MainMenuScene;
class Player;

class GameScene : public Scene {
private:
    std::vector<Player*> players;
    std::vector<GameObject*> mapV;
    std::vector<GameObject*> tiles;
    std::vector<GameObject*> objects;
	std::vector<TextRenderer*> ui_texts;

	TextRenderer* player1_text;
	TextRenderer* player2_text;
	TextRenderer* p1_bombs_text;
	TextRenderer* p2_bombs_text;
	TextRenderer* p1_bomb_timer_text;
	TextRenderer* p2_bomb_timer_text;
	TextRenderer* p1_walls_text;
	TextRenderer* p2_walls_text;
	TextRenderer* p1_wall_timer_text;
	TextRenderer* p2_wall_timer_text;
    Sprite* bomb_sprite;
	Sprite* wall_sprite;
    Sprite* speed_sprite;
	Sprite* radius_sprite;

    std::vector<std::vector<int>> tile_map;
    int* height_map;
    std::vector<std::vector<int>> object_map;
    std::vector<std::pair<int, int>> getAvailableTiles();

    bool walls_destroyed;
    bool gameEnded;

    CharacterType player1Character;
    CharacterType player2Character;

    const int width = 960;
    const int height = 832;
    const float tile_size = 64.0f;
    float random_bomb_timer;
    const float RANDOM_BOMB_INTERVAL = 20.0f;

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
    void spawnRandomBomb();
    void checkForWinner();
    void resetGame();

public:
    GameScene();
    ~GameScene() override;

    void initialize() override;
    void update(float deltaTime) override;
    void render() override;
    void cleanup() override;
    void onEnter() override;
    void onExit() override;

    void addBomb(int tile_x, int tile_y, int radius, int playerId);
    void addWall(int tile_x, int tile_y);
    void addExplosion(int tile_x, int tile_y, int radius, CharacterType character);
    void removeObjectAt(int tile_x, int tile_y);
    void setWallsDestroyed(bool destroyed) { walls_destroyed = destroyed; }

    void setPlayerCharacters(CharacterType p1Char, CharacterType p2Char);

    const std::vector<std::vector<int>>& getTileMap() const { return tile_map; }
    const std::vector<std::vector<int>>& getObjectMap() const { return object_map; }
    int* getHeightMap() const { return height_map; }
};