#include "MainMenuScene.h"
#include "SceneManager.h"
#include <freeglut.h>
#include <iostream>
#include "CharacterTypesEnum.h"

MainMenuScene::MainMenuScene() : Scene("MainMenu"),
currentState(MenuState::MAIN_MENU),
titleText(nullptr),
instructionText(nullptr),
player1Text(nullptr),
player2Text(nullptr),
selectedCharacterIndex(0),
player1Character(CharacterType::WHITE),
player2Character(CharacterType::BLACK),
player1Selected(false),
player2Selected(false),
selector(nullptr),
player1Indicator(nullptr),
player2Indicator(nullptr),
keyPressed(false) {
}

MainMenuScene::~MainMenuScene() {
    cleanup();
}

void MainMenuScene::initialize() {
    titleText = new TextRenderer(
        glm::vec2(140, 800),
        new Sprite("Resources/font.png", glm::vec2(64), 1, glm::vec2(15, 8), true),
        54, 32
    );
    titleText->setText("BOMBERMAN");

    instructionText = new TextRenderer(
        glm::vec2(360, 500),
        new Sprite("Resources/font.png", glm::vec2(32), 1, glm::vec2(15, 8), true),
        27, 32
    );
    instructionText->setText("PRESS_ENTER");

    player1Text = new TextRenderer(
        glm::vec2(300, 600),
        new Sprite("Resources/font.png", glm::vec2(48), 1, glm::vec2(15, 8), true),
        27, 32
    );
    player1Text->setText("PLAYER_1");

    player2Text = new TextRenderer(
        glm::vec2(300, 600),
        new Sprite("Resources/font.png", glm::vec2(48), 1, glm::vec2(15, 8), true),
        27, 32
    );
    player2Text->setText("PLAYER_2");

    selectCharacterText = new TextRenderer(
        glm::vec2(260, 450),
        new Sprite("Resources/font.png", glm::vec2(32), 1, glm::vec2(15, 8), true),
        27, 32
    );
    selectCharacterText->setText("SELECT_CHARACTER");

    menuControlsText = new TextRenderer(
        glm::vec2(50, 600),
        new Sprite("Resources/font.png", glm::vec2(32), 1, glm::vec2(15, 8), true),
        27, 32
    );
    menuControlsText->setText("A D_ENTER");

    controlsText1 = new TextRenderer(
        glm::vec2(600, 600),
        new Sprite("Resources/font.png", glm::vec2(32), 1, glm::vec2(15, 8), true),
        27, 32
    );
	controlsText1->setText("WASD_V");

    controlsText2 = new TextRenderer(
        glm::vec2(600, 600),
        new Sprite("Resources/font.png", glm::vec2(32), 1, glm::vec2(15, 8), true),
        27, 32
	);
	controlsText2->setText("IJKL_N");


    initializeCharacterPreviews();

    selector = new GameObject(
        glm::vec2(200, 370),
        glm::vec2(0),
        new Sprite("resources/arrow.png", glm::vec2(80), 1, glm::vec2(1), true)
    );
    selector->get_sprite()->set_current_frame(0);

    player1Indicator = new GameObject(
        glm::vec2(0, 0),
        glm::vec2(0),
        new Sprite("resources/arrow.png", glm::vec2(32), 1, glm::vec2(1), true)
    );

    player2Indicator = new GameObject(
        glm::vec2(0, 0),
        glm::vec2(0),
        new Sprite("resources/arrow.png", glm::vec2(32), 1, glm::vec2(1), true)
);

    initialized = true;
}

void MainMenuScene::initializeCharacterPreviews() {
    characterPreviews.clear();

    const float startX = 200.0f;
    const float startY = 300.0f;
    const float spacing = 150.0f;

    for (int i = 0; i < 4; i++) {
        CharacterType charType = static_cast<CharacterType>(i);
        GameObject* preview = new GameObject(
            glm::vec2(startX + i * spacing, startY),
            glm::vec2(0),
            new Sprite(getCharacterSpritePath(charType), glm::vec2(64), 1, glm::vec2(4, 3), true)
        );
        preview->get_sprite()->set_current_frame(1);
        characterPreviews.push_back(preview);
    }
}

const char* MainMenuScene::getCharacterSpritePath(CharacterType character) {
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

const char* MainMenuScene::getCharacterName(CharacterType character) {
    switch (character) {
    case CharacterType::WHITE:
        return "WHITE";
    case CharacterType::BLACK:
        return "BLACK";
    case CharacterType::BLUE:
        return "BLUE";
    case CharacterType::RED:
        return "RED";
    default:
        return "WHITE";
    }
}

bool MainMenuScene::isCharacterAvailable(CharacterType character) {
    if (currentState == MenuState::PLAYER1_SELECT) {
        return true;
    }
    else if (currentState == MenuState::PLAYER2_SELECT) {
        return character != player1Character;
    }
    return true;
}

void MainMenuScene::update(float deltaTime) {
    switch (currentState) {
    case MenuState::MAIN_MENU:
        updateMainMenu(deltaTime);
        break;
    case MenuState::PLAYER1_SELECT:
        updatePlayer1Selection(deltaTime);
        break;
    case MenuState::PLAYER2_SELECT:
        updatePlayer2Selection(deltaTime);
        break;
    case MenuState::START:
    {
        std::cout << "Starting game with:" << std::endl;
        std::cout << "  Player 1: " << getCharacterName(player1Character)
            << " (" << static_cast<int>(player1Character) << ")" << std::endl;
        std::cout << "  Player 2: " << getCharacterName(player2Character)
            << " (" << static_cast<int>(player2Character) << ")" << std::endl;

        SceneManager* sceneManager = SceneManager::getInstance();
        sceneManager->changeScene("Game");

        currentState = MenuState::MAIN_MENU;
    }
    break;
    }
}

void MainMenuScene::updateMainMenu(float deltaTime) {
    handleMainMenuInput();
}

void MainMenuScene::updatePlayer1Selection(float deltaTime) {
    const float startX = 200.0f;
    const float spacing = 150.0f;
    selector->set_position(glm::vec2(startX + selectedCharacterIndex * spacing - 8, 370));

    handlePlayer1SelectionInput();
}

void MainMenuScene::updatePlayer2Selection(float deltaTime) {
    const float startX = 200.0f;
    const float spacing = 150.0f;
    selector->set_position(glm::vec2(startX + selectedCharacterIndex * spacing - 8, 370));

    handlePlayer2SelectionInput();
}

void MainMenuScene::handleMainMenuInput() {

    if (Input::getKeyDown(13)) {
        currentState = MenuState::PLAYER1_SELECT;
        selectedCharacterIndex = 0;
    }
}

void MainMenuScene::handlePlayer1SelectionInput() {

    if (Input::getKeyDown('A') || Input::getKeyDown('a')) {
        selectedCharacterIndex = (selectedCharacterIndex - 1 + 4) % 4;
    }

    if (Input::getKeyDown('D') || Input::getKeyDown('d')) {
        selectedCharacterIndex = (selectedCharacterIndex + 1) % 4;
    }

    if (Input::getKeyDown(13)) {
        player1Character = static_cast<CharacterType>(selectedCharacterIndex);
        player1Selected = true;

        const float startX = 200.0f;
        const float spacing = 150.0f;
        player1Indicator->set_position(glm::vec2(startX + selectedCharacterIndex * spacing + 32, 340));

        currentState = MenuState::PLAYER2_SELECT;

        selectedCharacterIndex = 0;
        while (!isCharacterAvailable(static_cast<CharacterType>(selectedCharacterIndex)) && selectedCharacterIndex < 4) {
            selectedCharacterIndex++;
        }
    }
}

void MainMenuScene::handlePlayer2SelectionInput() {

    if (Input::getKeyDown('A') || Input::getKeyDown('a')) {
        do {
            selectedCharacterIndex = (selectedCharacterIndex - 1 + 4) % 4;
        } while (!isCharacterAvailable(static_cast<CharacterType>(selectedCharacterIndex)));
    }

    if (Input::getKeyDown('D') || Input::getKeyDown('d')) {
        do {
            selectedCharacterIndex = (selectedCharacterIndex + 1) % 4;
        } while (!isCharacterAvailable(static_cast<CharacterType>(selectedCharacterIndex)));
    }

    if (Input::getKeyDown(13)) { 
        player2Character = static_cast<CharacterType>(selectedCharacterIndex);
        player2Selected = true;

        const float startX = 200.0f;
        const float spacing = 150.0f;
        player2Indicator->set_position(glm::vec2(startX + selectedCharacterIndex * spacing + 32, 260));

        currentState = MenuState::START;
    }
}

void MainMenuScene::render() {
    glClear(GL_COLOR_BUFFER_BIT);

    switch (currentState) {
    case MenuState::MAIN_MENU:
        renderMainMenu();
        break;
    case MenuState::PLAYER1_SELECT:
        renderPlayer1Selection();
        break;
    case MenuState::PLAYER2_SELECT:
        renderPlayer2Selection();
        break;
    case MenuState::START:
        renderPlayer2Selection();
        break;
    }
}

void MainMenuScene::renderMainMenu() {
    if (titleText)
        titleText->render();

    if (instructionText)
        instructionText->render();
}

void MainMenuScene::renderPlayer1Selection() {
    if (titleText)
        titleText->render();

    if (player1Text)
        player1Text->render();

    if (selectCharacterText)
        selectCharacterText->render();

    if (controlsText1)
        controlsText1->render();

	if (menuControlsText)
		menuControlsText->render();

    for (auto& preview : characterPreviews) {
        if (preview) {
            preview->render();
        }
    }

    if (selector) {
        selector->render();
    }

    TextRenderer nameRenderer(
        glm::vec2(100, 220),
        new Sprite("Resources/font.png", glm::vec2(24), 1, glm::vec2(15, 8), true),
        20, 32
    );

    for (int i = 0; i < 4; i++) {
        const float startX = 180.0f;
        const float spacing = 150.0f;
        nameRenderer = TextRenderer(
            glm::vec2(startX + i * spacing, 220),
            new Sprite("Resources/font.png", glm::vec2(24), 1, glm::vec2(15, 8), true),
            20, 32
        );
        nameRenderer.setText(getCharacterName(static_cast<CharacterType>(i)));
        nameRenderer.render();
    }
}

void MainMenuScene::renderPlayer2Selection() {
    if (titleText)
        titleText->render();

    if (player2Text)
        player2Text->render();

    if (selectCharacterText)
        selectCharacterText->render();

    if (controlsText2)
		controlsText2->render();

    if (menuControlsText)
		menuControlsText->render();

    for (int i = 0; i < characterPreviews.size(); i++) {
        if (characterPreviews[i]) {
            if (!isCharacterAvailable(static_cast<CharacterType>(i)) && currentState == MenuState::PLAYER2_SELECT) {
                glColor3f(0.5f, 0.5f, 0.5f);
            }
            else {
                glColor3f(1.0f, 1.0f, 1.0f);
            }
            characterPreviews[i]->render();
            glColor3f(1.0f, 1.0f, 1.0f);
        }
    }

    if (selector && currentState == MenuState::PLAYER2_SELECT) {
        selector->render();
    }

    if (player1Selected && player1Indicator) {
        player1Indicator->render();
    }
    if (player2Selected && player2Indicator) {
        player2Indicator->render();
    }

    TextRenderer nameRenderer(
        glm::vec2(100, 220),
        new Sprite("Resources/font.png", glm::vec2(24), 1, glm::vec2(15, 8), true),
        20, 32
    );

    for (int i = 0; i < 4; i++) {
        const float startX = 180.0f;
        const float spacing = 150.0f;
        nameRenderer = TextRenderer(
            glm::vec2(startX + i * spacing, 220),
            new Sprite("Resources/font.png", glm::vec2(24), 1, glm::vec2(15, 8), true),
            20, 32
        );
        nameRenderer.setText(getCharacterName(static_cast<CharacterType>(i)));
        nameRenderer.render();
    }
}

void MainMenuScene::cleanup() {
    delete titleText;
    titleText = nullptr;

    delete instructionText;
    instructionText = nullptr;

    delete player1Text;
    player1Text = nullptr;

    delete player2Text;
    player2Text = nullptr;

    for (auto& preview : characterPreviews) {
        delete preview;
    }
    characterPreviews.clear();

    delete selector;
    selector = nullptr;

    delete player1Indicator;
    player1Indicator = nullptr;

    delete player2Indicator;
    player2Indicator = nullptr;
}

void MainMenuScene::onEnter() {
	AudioManager::stopMusic();
	AudioManager::playSong("menuSong");

    currentState = MenuState::MAIN_MENU;
    selectedCharacterIndex = 0;
    player1Selected = false;
    player2Selected = false;
    player1Character = CharacterType::WHITE;
    player2Character = CharacterType::BLACK;

    SceneManager* sceneManager = SceneManager::getInstance();
    GameScene* gameScene = dynamic_cast<GameScene*>(sceneManager->getScene("Game"));
}
