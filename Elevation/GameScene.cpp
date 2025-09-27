#include "GameScene.h"
#include "Bomb.h"
#include "Explosion.h"
#include "ObjectsEnum.h"
#include "SceneManager.h"
#include <iostream>
#include <cstdlib>
#include <ctime>  
#include "GameInstance.h"
#include "WinScene.h"

GameScene::GameScene() : Scene("Game"),
height_map(nullptr),
walls_destroyed(false),
random_bomb_timer(0.0f),
gameEnded(false) {
    player1Character = CharacterType::WHITE;
    player2Character = CharacterType::BLACK;
}


GameScene::~GameScene() {
    cleanup();
}

void GameScene::initialize() {
    player1Character = CharacterType::WHITE;
    player2Character = CharacterType::BLACK;

    initializeMaps();

    GameInstance::getInstance()->setCurrentGameScene(this);

    initialized = true;
}

void GameScene::initializeMaps() {
    tile_map = {
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, Wall::RADIUS, 0, 0, 0, 0, 0, Wall::BREAKABLE, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, Wall::BREAKABLE, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, Wall::UNBREAKABLE, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, Wall::SPEED, 0},
       {0, Wall::SPEED, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, Wall::UNBREAKABLE, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, Wall::BREAKABLE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, Wall::BREAKABLE, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    height_map = new int[195] {
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2,
        2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
        2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
        2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
        2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 2, 2,
        2, 2, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
        2, 1, 2, 1, 2, 2, 2, 1, 2, 1, 2, 1, 2, 1, 2,
        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
        2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2, 2,
        2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
        };

    object_map = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    int rows = static_cast<int>(tile_map.size());
    int cols = static_cast<int>(tile_map[0].size());

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int flipped_row = rows - 1 - row;
            if (tile_map[row][col] > 0) {
                int index = flipped_row * cols + col;
                height_map[index] = 2;
            }
        }
    }

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int flipped_row = rows - 1 - row;
            if (object_map[row][col] > 0) {
                int index = flipped_row * cols + col;
                height_map[index] = 2;
            }
        }
    }
}

bool GameScene::loadTextures() {
     for (auto& m : mapV) delete m;
    mapV.clear();
    for (auto& t : tiles) delete t;
    tiles.clear();
    for (auto& o : objects) delete o;
    objects.clear();
    for (auto& p : players) delete p;
    players.clear();

    Player* player1 = new Player(
        glm::vec2(64, 64),
        glm::vec2(0),
        new Sprite(
            getCharacterSpritePath(player1Character),
            glm::vec2(tile_size),
            1,
            glm::vec2(4, 3)
        ),
        height_map,
        &object_map,
        1
    );
    player1->get_sprite()->set_current_frame(1);
    players.push_back(player1);

    Player* player2 = new Player(
        glm::vec2(64 * 13, 64 * 11),
        glm::vec2(0),
        new Sprite(
            getCharacterSpritePath(player2Character),
            glm::vec2(tile_size),
            1,
            glm::vec2(4, 3)
        ),
        height_map,
        &object_map,
        2
    );
    player2->get_sprite()->set_current_frame(1);
    players.push_back(player2);

    GameObject* map = new GameObject(
        glm::vec2(0, 0),
        glm::vec2(0),
        new Sprite(
            "resources/level.png",
            glm::vec2(width, height),
            1,
            glm::vec2(1)
        )
    );
    map->get_sprite()->set_current_frame(0);
    mapV.push_back(map);

    for (int row = 0; row < tile_map.size(); ++row) {
        for (int col = 0; col < tile_map[row].size(); ++col) {
            float origin_x = (width - tile_map[0].size() * tile_size) / 2.0f;
            float origin_y = (height - tile_map.size() * tile_size) / 2.0f;
            int flipped_row = tile_map.size() - 1 - row;

            glm::vec2 pos(
                col * tile_size + origin_x,
                flipped_row * tile_size + origin_y
            );

            int wall_type = tile_map[row][col];

            if (wall_type == Wall::BREAKABLE) {
                GameObject* tile = new GameObject(
                    pos,
                    glm::vec2(0),
                    new Sprite(
                        "resources/walls.png",
                        glm::vec2(tile_size),
                        1,
                        glm::vec2(2, 1)
                    )
                );
                tile->get_sprite()->set_current_frame(1);
                tiles.push_back(tile);
            }
            else if (wall_type == Wall::UNBREAKABLE) {
                GameObject* tile = new GameObject(
                    pos,
                    glm::vec2(0),
                    new Sprite(
                        "resources/walls.png",
                        glm::vec2(tile_size),
                        1,
                        glm::vec2(2, 1)
                    )
                );
                tile->get_sprite()->set_current_frame(0);
                tiles.push_back(tile);
            }
            else if (wall_type == Wall::SPEED) {
                GameObject* tile = new GameObject(
                    pos,
                    glm::vec2(0),
                    new Sprite(
                        "resources/pickupSpeedWall.png",
                        glm::vec2(tile_size),
                        1,
                        glm::vec2(1)
                    )
                );
                tile->get_sprite()->set_current_frame(0);
                tiles.push_back(tile);
            }
            else if (wall_type == Wall::RADIUS) {
                GameObject* tile = new GameObject(
                    pos,
                    glm::vec2(0),
                    new Sprite(
                        "resources/pickupRadiusWall.png",
                        glm::vec2(tile_size),
                        1,
                        glm::vec2(1)
                    )
                );
                tile->get_sprite()->set_current_frame(0);
                tiles.push_back(tile);
            }
        }
    }

    return true;
}

void GameScene::update(float deltaTime) {
    for (auto& p : players) {
        if (p && p->get_is_active()) {
            p->update(deltaTime);
        }
    }

    updateObjects(deltaTime);

    cleanupInactiveObjects();

    if (walls_destroyed) {
        rebuildTiles();
        walls_destroyed = false;
    }

    random_bomb_timer += deltaTime;
    if (random_bomb_timer >= RANDOM_BOMB_INTERVAL) {
        spawnRandomBomb();
        random_bomb_timer = 0.0f;
    }

    checkForWinner();
}

void GameScene::updateObjects(float deltaTime) {
    for (size_t i = 0; i < objects.size(); ++i) {
        if (objects[i] && objects[i]->get_is_active()) {
            objects[i]->update(deltaTime);
        }
    }
}

void GameScene::cleanupInactiveObjects() {
    auto pl = players.begin();
    while (pl != players.end()) {
        if (!(*pl)->get_is_active()) {
            delete* pl;
            pl = players.erase(pl);
        }
        else {
            ++pl;
        }
    }

    auto t = tiles.begin();
    while (t != tiles.end()) {
        if (!(*t)->get_is_active()) {
            delete* t;
            t = tiles.erase(t);
        }
        else {
            ++t;
        }
    }

    auto ob = objects.begin();
    while (ob != objects.end()) {
        if (!(*ob)->get_is_active()) {
            delete* ob;
            ob = objects.erase(ob);
        }
        else {
            ++ob;
        }
    }
}

void GameScene::render() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto& m : mapV) {
        m->render();
    }

    for (auto& t : tiles) {
        if (t->get_is_active() && t->get_is_visible()) {
            t->render();
        }
    }

    for (auto& o : objects) {
        if (o->get_is_active() && o->get_is_visible()) {
            o->render();
        }
    }

    for (auto& p : players) {
        if (p->get_is_active() && p->get_is_visible()) {
            p->render();
        }
    }
}

void GameScene::cleanup() {
    for (auto& object : objects) {
        delete object;
    }
    objects.clear();

    for (auto& tile : tiles) {
        delete tile;
    }
    tiles.clear();

    for (auto& map : mapV) {
        delete map;
    }
    mapV.clear();

    for (auto& player : players) {
        delete player;
    }
    players.clear();

    if (height_map) {
        delete[] height_map;
        height_map = nullptr;
    }
}


void GameScene::onEnter() {
	AudioManager::stopMusic();
	AudioManager::playSong("gameSong");
    
    GameInstance::getInstance()->setCurrentGameScene(this);

    SceneManager* sceneManager = SceneManager::getInstance();
    MainMenuScene* mainMenu = dynamic_cast<MainMenuScene*>(sceneManager->getScene("MainMenu"));

    if (mainMenu) {
        CharacterType p1Char = mainMenu->getPlayer1Character();
        CharacterType p2Char = mainMenu->getPlayer2Character();

        player1Character = p1Char;
        player2Character = p2Char;
    }

    resetGame();
}

void GameScene::onExit() {
    GameInstance::getInstance()->setCurrentGameScene(nullptr);
}

void GameScene::checkForWinner() {
    if (gameEnded) {
        return;
    }

    int activePlayers = 0;
    Player* survivor = nullptr;
    int survivorId = 0;

    for (auto& player : players) {
        if (player && player->get_is_active()) {
            activePlayers++;
            survivor = player;
            survivorId = player->get_player_id();
        }
    }

    if (activePlayers == 1 && survivor) {
        gameEnded = true;
        std::cout << "Player " << survivorId << " wins!" << std::endl;

        CharacterType winnerCharacterType;
        if (survivorId == 1) {
            winnerCharacterType = player1Character;
        }
        else {
            winnerCharacterType = player2Character;
        }

        SceneManager* sceneManager = SceneManager::getInstance();
        WinScene* winScene = dynamic_cast<WinScene*>(sceneManager->getScene("Win"));
        if (winScene) {
            winScene->setWinner(survivorId, winnerCharacterType);
        }
        sceneManager->changeScene("Win");
    }
    else if (activePlayers == 0) {
        gameEnded = true;
        std::cout << "It's a draw! Both players died." << std::endl;

        SceneManager* sceneManager = SceneManager::getInstance();
        WinScene* winScene = dynamic_cast<WinScene*>(sceneManager->getScene("Win"));
        if (winScene) {
            winScene->setDraw(player1Character, player2Character);
        }
        sceneManager->changeScene("Win");
    }
}

void GameScene::resetGame() {

    gameEnded = false;

    cleanup();

    initializeMaps();

    walls_destroyed = false;
    random_bomb_timer = 0.0f;

    loadTextures();
}

std::vector<std::pair<int, int>> GameScene::getAvailableTiles() {
    std::vector<std::pair<int, int>> availableTiles;
    const int cols = 15;
    const int rows = 13;

    if (!height_map) {
        std::cout << "Height map is null." << std::endl;
        return availableTiles;
    }

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            int height_map_index = x + y * cols;

            if (height_map[height_map_index] == 1) {
               
                int object_map_y = rows - 1 - y;

                if (object_map_y >= 0 && object_map_y < static_cast<int>(object_map.size()) &&
                    x >= 0 && x < static_cast<int>(object_map[object_map_y].size())) {

                    if (object_map[object_map_y][x] == 0) {
                        bool playerOnTile = false;
                        float tile_center_x = x * tile_size + (960 - cols * tile_size) / 2.0f + tile_size / 2;
                        float tile_center_y = y * tile_size + (832 - rows * tile_size) / 2.0f + tile_size / 2;

                        for (const auto& player : players) {
                            if (player && player->get_is_active()) {
                                glm::vec2 player_pos = player->get_position();
                                glm::vec2 player_center = player_pos + glm::vec2(32, 32);

                                float distance = glm::distance(player_center, glm::vec2(tile_center_x, tile_center_y));
                                if (distance < tile_size / 2 + 10) {
                                    playerOnTile = true;
                                    break;
                                }
                            }
                        }

                        if (!playerOnTile) {
                            availableTiles.push_back(std::make_pair(x, y));
                        }
                    }
                }
            }
        }
    }

    return availableTiles;
}

const char* GameScene::getBombSpritePath(CharacterType character) {
    switch (character) {
    case CharacterType::WHITE:
        return "resources/bombWhite.png";
    case CharacterType::BLACK:
        return "resources/bombBlack.png";
    case CharacterType::BLUE:
        return "resources/bombBlue.png";
    case CharacterType::RED:
        return "resources/bombRed.png";
    default:
        return "resources/bombBlack.png";
    }
}

const char* GameScene::getExplosionSpritePath(CharacterType character) {
    switch (character) {
    case CharacterType::WHITE:
        return "resources/explosionWhite.png";
    case CharacterType::BLACK:
        return "resources/explosionBlack.png";
    case CharacterType::BLUE:
        return "resources/explosionBlue.png";
    case CharacterType::RED:
        return "resources/explosionRed.png";
    default:
        return "resources/explosionBlack.png";
    }
}

CharacterType GameScene::getPlayerCharacterById(int playerId) {
    if (playerId == 1) {
        return player1Character;
    }
    else if (playerId == 2) {
        return player2Character;
    }
    return CharacterType::WHITE;
}

void GameScene::addBomb(int tile_x, int tile_y, int radius, int playerId) {
    if (tile_x < 0 || tile_x >= 15 || tile_y < 0 || tile_y >= 13) {
        return;
    }

    int tile_y_reversed = 12 - tile_y;

    if (object_map[tile_y_reversed][tile_x] != 0) {
        return;
    }

    object_map[tile_y_reversed][tile_x] = Object::BOMB;

    float origin_x = (width - 15 * tile_size) / 2.0f;
    float origin_y = (height - 13 * tile_size) / 2.0f;

    glm::vec2 bomb_pos(
        tile_x * tile_size + origin_x,
        tile_y * tile_size + origin_y
    );

    CharacterType character = getPlayerCharacterById(playerId);

    Bomb* new_bomb = new Bomb(
        bomb_pos,
        glm::vec2(0),
        new Sprite(
            getBombSpritePath(character),
            glm::vec2(tile_size),
            1,
            glm::vec2(3, 1)
        ),
        3.0f,
        &object_map,
        tile_x,
        tile_y_reversed,
        radius,
        character
    );
    new_bomb->get_sprite()->set_current_frame(2);
    objects.push_back(new_bomb);

    std::cout << "Bomb created at world tile (" << tile_x << ", " << tile_y
        << ") array pos (" << tile_x << ", " << tile_y_reversed << ") for character "
        << static_cast<int>(character) << std::endl;
}

void GameScene::spawnRandomBomb() {
    std::vector<std::pair<int, int>> availableTiles = getAvailableTiles();

    if (availableTiles.empty()) {
        std::cout << "No available tiles for random bomb" << std::endl;
        return;
    }

    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned int>(time(nullptr)));
        seeded = true;
    }

    int randomIndex = rand() % availableTiles.size();
    std::pair<int, int> selectedTile = availableTiles[randomIndex];

    int tile_x = selectedTile.first;
    int tile_y = selectedTile.second;

    int world_tile_y = 12 - tile_y;

    std::cout << "Spawning random bomb at tile (" << tile_x << ", " << world_tile_y << ")" << std::endl;

    int randomColor = rand() % 4;
    int radius = 2;

    addBomb(tile_x, world_tile_y, radius, randomColor);
}

void GameScene::addExplosion(int tile_x, int tile_y, int radius, CharacterType character) {
    if (tile_x < 0 || tile_x >= 15 || tile_y < 0 || tile_y >= 13) {
        std::cout << "Explosion coordinates out of bounds" << std::endl;
        return;
    }

    int tile_y_reversed = 12 - tile_y;
    std::cout << "Explosion created at world tile (" << tile_x << ", " << tile_y
        << ") array pos (" << tile_x << ", " << tile_y_reversed << ") for character "
        << static_cast<int>(character) << std::endl;

    float origin_x = (width - 15 * tile_size) / 2.0f;
    float origin_y = (height - 13 * tile_size) / 2.0f;

    glm::vec2 explosion_pos(
        tile_x * tile_size + origin_x,
        tile_y * tile_size + origin_y
    );

    std::cout << "Creating explosion at world pos (" << explosion_pos.x << ", " << explosion_pos.y << ")" << std::endl;

    Explosion* new_explosion = new Explosion(
        explosion_pos,
        glm::vec2(0),
        new Sprite(
            getExplosionSpritePath(character),
            glm::vec2(tile_size),
            1,
            glm::vec2(7, 1)
        ),
        radius,
        &object_map,
        &tile_map,
        tile_x,
        tile_y_reversed,
        height_map
    );
    new_explosion->get_sprite()->set_current_frame(0);
    objects.push_back(new_explosion);
}

void GameScene::removeObjectAt(int tile_x, int tile_y)
{
    if (tile_x < 0 || tile_x >= object_map[0].size() || tile_y < 0 || tile_y >= object_map.size())
        return;

    object_map[tile_y][tile_x] = 0;

    auto it = objects.begin();
    while (it != objects.end()) {
        glm::vec2 obj_pos = (*it)->get_position();
        float origin_x = (width - object_map[0].size() * tile_size) / 2.0f;
        float origin_y = (height - object_map.size() * tile_size) / 2.0f;
        int obj_x = static_cast<int>((obj_pos.x - origin_x) / tile_size);
        int obj_y = static_cast<int>((obj_pos.y - origin_y) / tile_size);

        if (obj_x == tile_x && obj_y == tile_y) {
            delete *it;
            it = objects.erase(it);
        } else {
            ++it;
        }
    }
}

void GameScene::rebuildTiles() {
    for (auto& t : tiles) delete t;
    tiles.clear();

    for (int row = 0; row < tile_map.size(); ++row) {
        for (int col = 0; col < tile_map[row].size(); ++col) {
            float origin_x = (width - tile_map[0].size() * tile_size) / 2.0f;
            float origin_y = (height - tile_map.size() * tile_size) / 2.0f;
            int flipped_row = tile_map.size() - 1 - row;

            glm::vec2 pos(
                col * tile_size + origin_x,
                flipped_row * tile_size + origin_y
            );

            int wall_type = tile_map[row][col];

            if (wall_type == Wall::BREAKABLE) {
                GameObject* tile = new GameObject(
                    pos,
                    glm::vec2(0),
                    new Sprite(
                        "resources/walls.png",
                        glm::vec2(tile_size),
                        1,
                        glm::vec2(2, 1)
                    )
                );
                tile->get_sprite()->set_current_frame(1);
                tiles.push_back(tile);
            }
            else if (wall_type == Wall::UNBREAKABLE) {
                GameObject* tile = new GameObject(
                    pos,
                    glm::vec2(0),
                    new Sprite(
                        "resources/walls.png",
                        glm::vec2(tile_size),
                        1,
                        glm::vec2(2, 1)
                    )
                );
                tile->get_sprite()->set_current_frame(0);
                tiles.push_back(tile);
            }
            else if (wall_type == Wall::SPEED) {
                GameObject* tile = new GameObject(
                    pos,
                    glm::vec2(0),
                    new Sprite(
                        "resources/pickupSpeedWall.png",
                        glm::vec2(tile_size),
                        1,
                        glm::vec2(1)
                    )
                );
                tile->get_sprite()->set_current_frame(0);
                tiles.push_back(tile);
            }
            else if (wall_type == Wall::RADIUS) {
                GameObject* tile = new GameObject(
                    pos,
                    glm::vec2(0),
                    new Sprite(
                        "resources/pickupRadiusWall.png",
                        glm::vec2(tile_size),
                        1,
                        glm::vec2(1)
                    )
                );
                tile->get_sprite()->set_current_frame(0);
                tiles.push_back(tile);
            }
        }
    }

    for (int row = 0; row < object_map.size(); ++row) {
        for (int col = 0; col < object_map[row].size(); ++col) {
            float origin_x = (width - object_map[0].size() * tile_size) / 2.0f;
            float origin_y = (height - object_map.size() * tile_size) / 2.0f;

            glm::vec2 pos(
                col * tile_size + origin_x,
                row * tile_size + origin_y
            );

            int object_type = object_map[row][col];

            if (object_type == Object::PICKUP_SPEED) {
                GameObject* object = new GameObject(
                    pos,
                    glm::vec2(0),
                    new Sprite(
                        "resources/powerUpLR.png",
                        glm::vec2(tile_size),
                        1,
                        glm::vec2(1)
                    )
                );
                object->get_sprite()->set_current_frame(0);
                objects.push_back(object);
            }
            else if (object_type == Object::PICKUP_RADIUS) {
                GameObject* object = new GameObject(
                    pos,
                    glm::vec2(0),
                    new Sprite(
                        "resources/powerUpUD.png",
                        glm::vec2(tile_size),
                        1,
                        glm::vec2(1)
                    )
                );
                object->get_sprite()->set_current_frame(0);
                objects.push_back(object);
            }
        }
    }
}

const char* GameScene::getCharacterSpritePath(CharacterType character) {
    switch (character) {
    case CharacterType::WHITE:
        return "resources/charWhite.png";
    case CharacterType::BLACK:
        return "resources/charBlack.png";
    case CharacterType::BLUE:
        return "resources/charBlue.png";
    case CharacterType::RED:
        return "resources/charRed.png";
    default:
        return "resources/charWhite.png";
    }
}

void GameScene::setPlayerCharacters(CharacterType p1Char, CharacterType p2Char) {
    player1Character = p1Char;
    player2Character = p2Char;

    if (initialized && GameInstance::getInstance()->getCurrentGameScene() == this) {
        initializePlayersWithCharacters();
    }
}

void GameScene::initializePlayersWithCharacters() {
    for (auto& p : players) {
        delete p;
    }
    players.clear();

    Player* player1 = new Player(
        glm::vec2(64, 64),
        glm::vec2(0),
        new Sprite(
            getCharacterSpritePath(player1Character),
            glm::vec2(tile_size),
            1,
            glm::vec2(4, 3)
        ),
        height_map,
        &object_map,
        1
    );
    player1->get_sprite()->set_current_frame(0);
    players.push_back(player1);

    Player* player2 = new Player(
        glm::vec2(832, 704),
        glm::vec2(0),
        new Sprite(
            getCharacterSpritePath(player2Character),
            glm::vec2(tile_size),
            1,
            glm::vec2(4, 3)
        ),
        height_map,
        &object_map,
        2
    );
    player2->get_sprite()->set_current_frame(0);
    players.push_back(player2);
}