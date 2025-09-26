#include "GameInstance.h"
#include "GameScene.h"

GameInstance* GameInstance::instance = nullptr;

GameInstance* GameInstance::getInstance() {
    if (instance == nullptr) {
        instance = new GameInstance();
    }
    return instance;
}

GameScene* GameInstance::getCurrentGameScene()
{
    return currentGameScene;
}

void GameInstance::setCurrentGameScene(GameScene* scene) {
    currentGameScene = scene;
}