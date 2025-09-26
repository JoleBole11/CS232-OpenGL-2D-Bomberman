#include "MainMenuScene.h"

MainMenuScene::MainMenuScene()
	: Scene("MainMenu"), title() {
	
}

MainMenuScene::~MainMenuScene()
{
	cleanup();
}

void MainMenuScene::initialize()
{
    title = new TextRenderer(
        glm::vec2(30, 700),
        new Sprite("Resources/font.png", glm::vec2(64), 1, glm::vec2(15, 8), true),
        54, 32
    );
    title->setText("BOMBERMAN");

	buttons.push_back(new Button(glm::vec2(400, 400), glm::vec2(300, 70),
		"WHITE", new Sprite("resources/charWhite.png", glm::vec2(128), 1, glm::vec2(4, 3)), glm::vec3(0, 0.8f, 0)));
	buttons.push_back(new Button(glm::vec2(400, 100), glm::vec2(300, 70),
		"BLACK", new Sprite("Resources/charBlack.png", glm::vec2(128), 1, glm::vec2(4, 3)), glm::vec3(0.8f, 0, 0)));
	buttons.push_back(new Button(glm::vec2(400, 400), glm::vec2(300, 70),
		"BLUE", new Sprite("Resources/charBlue.png", glm::vec2(128), 1, glm::vec2(4, 3)), glm::vec3(0, 0.8f, 0)));
	buttons.push_back(new Button(glm::vec2(400, 100), glm::vec2(300, 70),
		"RED", new Sprite("Resources/charRed.png", glm::vec2(128), 1, glm::vec2(4, 3)), glm::vec3(0.8f, 0, 0)));

    initialized = true;
}

void MainMenuScene::update(float deltaTime)
{
	for (auto button : buttons) {
		button->update(deltaTime);
	}

	buttons[0]->onClick();
}

void MainMenuScene::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (title) {
		title->render();
	}
	for (auto button : buttons) {
		button->render();
	}
}

void MainMenuScene::cleanup()
{
	delete title;
	title = nullptr;
	for (auto button : buttons) {
		delete button;
	}
	buttons.clear();
}

void MainMenuScene::onEnter()
{
}
