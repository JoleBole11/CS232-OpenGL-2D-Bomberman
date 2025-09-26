#pragma once

class GameScene;

class GameInstance {
private:
    static GameInstance* instance;
    GameScene* currentGameScene;

    GameInstance() : currentGameScene(nullptr) {}

public:
    static GameInstance* getInstance();
    GameScene* getCurrentGameScene() const { return currentGameScene; }

    void setCurrentGameScene(GameScene* scene);


    ~GameInstance() = default;

    // Delete copy constructor and assignment operator
    GameInstance(const GameInstance&) = delete;
    GameInstance& operator=(const GameInstance&) = delete;
};