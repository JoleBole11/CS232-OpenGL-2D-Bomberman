#include "IntroScene.h"
#include "SceneManager.h"
#include <freeglut.h>

IntroScene::IntroScene() : Scene("Intro"), introText(nullptr), duration(3.0f), timer(0.0f) {
}

IntroScene::~IntroScene() {
    cleanup();
}

void IntroScene::initialize() {
    introText = new TextRenderer(
        glm::vec2(30, 700),
        new Sprite("Resources/font.png", glm::vec2(64), 1, glm::vec2(15, 8), true),
        54, 32
    );
    introText->setText("BOMBERMAN_____BY__BOGDAN_CVETKOVIC_5934");

    initialized = true;
}

void IntroScene::update(float deltaTime) {
    timer += deltaTime;

    if (timer >= duration) {
        SceneManager::getInstance()->changeScene("Game");
    }
}

void IntroScene::render() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (introText) {
        introText->render();
    }
}

void IntroScene::cleanup() {
    delete introText;
    introText = nullptr;
}

void IntroScene::onEnter() {
    timer = 0.0f;
}