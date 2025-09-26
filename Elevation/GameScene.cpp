#include "GameScene.h"
#include "Bomb.h"
#include "Explosion.h"
#include "ObjectsEnum.h"
#include "SceneManager.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "GameInstance.h"

// GameScene implementation
GameScene::GameScene() : Scene("Game"), height_map(nullptr), walls_destroyed(false) {
}


GameScene::~GameScene() {
    cleanup();
}

void GameScene::initialize() {
    initializeMaps();
    loadTextures();

    // Register this scene with GameInstance
    GameInstance::getInstance()->setCurrentGameScene(this);

    initialized = true;
}

void GameScene::initializeMaps() {
    // Initialize tile map
    tile_map = {
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
       {0, 0, 0, 0, 0, 0, 0, Wall::RADIUS, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    // Initialize height map
    height_map = new int[195] {
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
            2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
            2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
            2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
            2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
            2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
            2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
            2, 2, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
            2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
            2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
            2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
        };

    // Initialize object map
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

    // Update height map based on tile and object maps
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
    // Clear existing objects
    for (auto& m : mapV) delete m;
    mapV.clear();
    for (auto& t : tiles) delete t;
    tiles.clear();
    for (auto& o : objects) delete o;
    objects.clear();
    for (auto& p : players) delete p;
    players.clear();

    // Create player
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
        glm::vec2(64 * 13, 64 * 15),
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
    player1->get_sprite()->set_current_frame(0);
    players.push_back(player1);

    // Create background map
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

    // Create tiles from tile_map
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
    // Update players
    for (auto& p : players) {
        p->update(deltaTime);
    }

    // Update objects
    updateObjects(deltaTime);

    // Clean up inactive objects
    cleanupInactiveObjects();

    // Rebuild tiles if walls were destroyed
    if (walls_destroyed) {
        rebuildTiles();
        walls_destroyed = false;
    }
}

void GameScene::updateObjects(float deltaTime) {
    for (auto& o : objects) {
        o->update(deltaTime);
    }
}

void GameScene::cleanupInactiveObjects() {
    // Clean up players
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

    // Clean up tiles
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

    // Clean up objects
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

    // Render background
    for (auto& m : mapV) {
        m->render();
    }

    // Render tiles
    for (auto& t : tiles) {
        if (t->get_is_active() && t->get_is_visible()) {
            t->render();
        }
    }

    // Render objects
    for (auto& o : objects) {
        if (o->get_is_active() && o->get_is_visible()) {
            o->render();
        }
    }

    // Render players
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

void GameScene::handleKeyboard(unsigned char key, int x, int y) {
    // Handle scene-specific keyboard input if needed
    // Most input is handled directly in Player::update() via Input class
}

void GameScene::onEnter() {
    // Register this scene with GameInstance when entering
    GameInstance::getInstance()->setCurrentGameScene(this);

    // Get character selections from MainMenuScene if available
    SceneManager* sceneManager = SceneManager::getInstance();
    MainMenuScene* mainMenu = dynamic_cast<MainMenuScene*>(sceneManager->getScene("MainMenu"));

    if (mainMenu) {
        CharacterType p1Char = mainMenu->getPlayer1Character();
        CharacterType p2Char = mainMenu->getPlayer2Character();

        // Set the character selections
        setPlayerCharacters(p1Char, p2Char);

        // Initialize players with selected characters
        initializePlayersWithCharacters();
    }
}

void GameScene::onExit() {
    // Unregister this scene from GameInstance when exiting
    GameInstance::getInstance()->setCurrentGameScene(nullptr);
}

void GameScene::addBomb(int tile_x, int tile_y, int radius) {
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

    Bomb* new_bomb = new Bomb(
        bomb_pos,
        glm::vec2(0),
        new Sprite(
            "resources/bombBlack.png",
            glm::vec2(tile_size),
            1,
            glm::vec2(3, 1)
        ),
        3.0f,
        &object_map,
        tile_x,
        tile_y_reversed,
        radius
    );
    new_bomb->get_sprite()->set_current_frame(2);
    objects.push_back(new_bomb);

    std::cout << "Bomb created at world tile (" << tile_x << ", " << tile_y
        << ") array pos (" << tile_x << ", " << tile_y_reversed << ")" << std::endl;
}

void GameScene::addExplosion(int tile_x, int tile_y, int radius) {
    if (tile_x < 0 || tile_x >= 15 || tile_y < 0 || tile_y >= 13) {
        std::cout << "Explosion coordinates out of bounds!" << std::endl;
        return;
    }

    int tile_y_reversed = 12 - tile_y;
    std::cout << "Explosion created at world tile (" << tile_x << ", " << tile_y
        << ") array pos (" << tile_x << ", " << tile_y_reversed << ")" << std::endl;

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
            "resources/explosionBlack.png",
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

    std::cout << "Explosion object created and added to objects vector" << std::endl;
}

void GameScene::rebuildTiles() {
    // Clear existing tiles
    for (auto& t : tiles) delete t;
    tiles.clear();

    // Rebuild tiles from tile_map
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

    // Add pickup objects from object_map
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

    // If already initialized, rebuild players with new characters
    if (initialized) {
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
        glm::vec2(64 * 13, 64 * 15),
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

void GameScene::removeObjectAt(int tile_x, int tile_y) {
    float origin_x = (width - 15 * tile_size) / 2.0f;
    float origin_y = (height - 13 * tile_size) / 2.0f;

    glm::vec2 expected_pos(
        tile_x * tile_size + origin_x,
        tile_y * tile_size + origin_y
    );

    for (auto& obj : objects) {
        glm::vec2 obj_pos = obj->get_position();

        if (abs(obj_pos.x - expected_pos.x) < 1.0f &&
            abs(obj_pos.y - expected_pos.y) < 1.0f) {
            obj->set_is_active(false);
            obj->set_is_visible(false);
            break;
        }
    }
}