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
    // Create title text
    titleText = new TextRenderer(
        glm::vec2(200, 700),
        new Sprite("Resources/font.png", glm::vec2(64), 1, glm::vec2(15, 8), true),
        54, 32
    );
    titleText->setText("BOMBERMAN");

    // Create instruction text - Updated to be clear about 2-player
    instructionText = new TextRenderer(
        glm::vec2(100, 500),
        new Sprite("Resources/font.png", glm::vec2(32), 1, glm::vec2(15, 8), true),
        27, 32
    );
    instructionText->setText("2_PLAYER_MULTIPLAYER___PRESS_ENTER");

    // Create player texts
    player1Text = new TextRenderer(
        glm::vec2(100, 600),
        new Sprite("Resources/font.png", glm::vec2(32), 1, glm::vec2(15, 8), true),
        27, 32
    );
    player1Text->setText("PLAYER_1__SELECT_CHARACTER___A_D_ENTER");

    player2Text = new TextRenderer(
        glm::vec2(100, 600),
        new Sprite("Resources/font.png", glm::vec2(32), 1, glm::vec2(15, 8), true),
        27, 32
    );
    player2Text->setText("PLAYER_2__SELECT_CHARACTER___A_D_ENTER");

    // Initialize character previews
    initializeCharacterPreviews();

    // Create selector (highlight box)
    selector = new GameObject(
        glm::vec2(200, 300),
        glm::vec2(0),
        new Sprite("resources/walls.png", glm::vec2(80), 1, glm::vec2(2, 1), true)
    );
    selector->get_sprite()->set_current_frame(1); // Use breakable wall sprite as highlight

    // Create player indicators
    player1Indicator = new GameObject(
        glm::vec2(0, 0),
        glm::vec2(0),
        new Sprite("resources/powerUpUD.png", glm::vec2(32), 1, glm::vec2(1), true)
    );

    player2Indicator = new GameObject(
        glm::vec2(0, 0),
        glm::vec2(0),
        new Sprite("resources/powerUpLR.png", glm::vec2(32), 1, glm::vec2(1), true)
    );

    initialized = true;
}

void MainMenuScene::initializeCharacterPreviews() {
    characterPreviews.clear();

    const float startX = 200.0f;
    const float startY = 300.0f;
    const float spacing = 150.0f;

    // Create character preview objects
    for (int i = 0; i < 4; i++) {
        CharacterType charType = static_cast<CharacterType>(i);
        GameObject* preview = new GameObject(
            glm::vec2(startX + i * spacing, startY),
            glm::vec2(0),
            new Sprite(getCharacterSpritePath(charType), glm::vec2(64), 1, glm::vec2(4, 3), true)
        );
        preview->get_sprite()->set_current_frame(1); // Standing frame
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
        return true; // Player 1 can select any character
    }
    else if (currentState == MenuState::PLAYER2_SELECT) {
        return character != player1Character; // Player 2 can't select Player 1's character
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
    case MenuState::READY_TO_START:
        // Pass the selected characters to GameScene before transitioning
        SceneManager* sceneManager = SceneManager::getInstance();
        // Get the GameScene and set the characters
        // Note: You might need to add a method to get a specific scene from SceneManager
        // For now, we'll transition directly
        sceneManager->changeScene("Game");
        break;
    }
}

void MainMenuScene::updateMainMenu(float deltaTime) {
    handleMainMenuInput();
}

void MainMenuScene::updatePlayer1Selection(float deltaTime) {
    // Update selector position
    const float startX = 200.0f;
    const float spacing = 150.0f;
    selector->set_position(glm::vec2(startX + selectedCharacterIndex * spacing - 8, 292));

    handlePlayer1SelectionInput();
}

void MainMenuScene::updatePlayer2Selection(float deltaTime) {
    // Update selector position
    const float startX = 200.0f;
    const float spacing = 150.0f;
    selector->set_position(glm::vec2(startX + selectedCharacterIndex * spacing - 8, 292));

    handlePlayer2SelectionInput();
}

void MainMenuScene::handleMainMenuInput() {
    static bool enterPressed = false;

    if (Input::getKey(13) && !enterPressed) { // Enter key
        enterPressed = true;
        currentState = MenuState::PLAYER1_SELECT;
        selectedCharacterIndex = 0;
    }

    if (!Input::getKey(13)) {
        enterPressed = false;
    }
}

void MainMenuScene::handlePlayer1SelectionInput() {
    static bool leftPressed = false;
    static bool rightPressed = false;
    static bool enterPressed = false;

    // Handle left/right navigation
    if (Input::getKey('A') || Input::getKey('a')) {
        if (!leftPressed) {
            selectedCharacterIndex = (selectedCharacterIndex - 1 + 4) % 4;
            leftPressed = true;
        }
    }
    else {
        leftPressed = false;
    }

    if (Input::getKey('D') || Input::getKey('d')) {
        if (!rightPressed) {
            selectedCharacterIndex = (selectedCharacterIndex + 1) % 4;
            rightPressed = true;
        }
    }
    else {
        rightPressed = false;
    }

    // Handle selection
    if (Input::getKey(13) && !enterPressed) { // Enter key
        player1Character = static_cast<CharacterType>(selectedCharacterIndex);
        player1Selected = true;

        // Set player 1 indicator position
        const float startX = 200.0f;
        const float spacing = 150.0f;
        player1Indicator->set_position(glm::vec2(startX + selectedCharacterIndex * spacing + 32, 340));

        // Move to player 2 selection
        currentState = MenuState::PLAYER2_SELECT;

        // Find first available character for player 2
        selectedCharacterIndex = 0;
        while (!isCharacterAvailable(static_cast<CharacterType>(selectedCharacterIndex)) && selectedCharacterIndex < 4) {
            selectedCharacterIndex++;
        }

        enterPressed = true;
    }

    if (!Input::getKey(13)) {
        enterPressed = false;
    }
}

void MainMenuScene::handlePlayer2SelectionInput() {
    static bool leftPressed = false;
    static bool rightPressed = false;
    static bool enterPressed = false;

    // Handle left/right navigation
    if (Input::getKey('A') || Input::getKey('a')) {
        if (!leftPressed) {
            do {
                selectedCharacterIndex = (selectedCharacterIndex - 1 + 4) % 4;
            } while (!isCharacterAvailable(static_cast<CharacterType>(selectedCharacterIndex)));
            leftPressed = true;
        }
    }
    else {
        leftPressed = false;
    }

    if (Input::getKey('D') || Input::getKey('d')) {
        if (!rightPressed) {
            do {
                selectedCharacterIndex = (selectedCharacterIndex + 1) % 4;
            } while (!isCharacterAvailable(static_cast<CharacterType>(selectedCharacterIndex)));
            rightPressed = true;
        }
    }
    else {
        rightPressed = false;
    }

    // Handle selection
    if (Input::getKey(13) && !enterPressed) { // Enter key
        player2Character = static_cast<CharacterType>(selectedCharacterIndex);
        player2Selected = true;

        // Set player 2 indicator position
        const float startX = 200.0f;
        const float spacing = 150.0f;
        player2Indicator->set_position(glm::vec2(startX + selectedCharacterIndex * spacing + 32, 260));

        // Ready to start game
        currentState = MenuState::READY_TO_START;

        enterPressed = true;
    }

    if (!Input::getKey(13)) {
        enterPressed = false;
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
    case MenuState::READY_TO_START:
        renderPlayer2Selection(); // Show final selection
        break;
    }
}

void MainMenuScene::renderMainMenu() {
    if (titleText) {
        titleText->render();
    }
    if (instructionText) {
        instructionText->render();
    }

    // Add some visual indication that this is multiplayer only
    TextRenderer multiplayerNote(
        glm::vec2(150, 400),
        new Sprite("Resources/font.png", glm::vec2(24), 1, glm::vec2(15, 8), true),
        20, 32
    );
    multiplayerNote.setText("TWO_PLAYERS_REQUIRED");
    multiplayerNote.render();
}

void MainMenuScene::renderPlayer1Selection() {
    if (titleText) {
        titleText->render();
    }
    if (player1Text) {
        player1Text->render();
    }

    // Render character previews
    for (auto& preview : characterPreviews) {
        if (preview) {
            preview->render();
        }
    }

    // Render selector
    if (selector) {
        selector->render();
    }

    // Render character names
    TextRenderer nameRenderer(
        glm::vec2(100, 200),
        new Sprite("Resources/font.png", glm::vec2(24), 1, glm::vec2(15, 8), true),
        20, 32
    );

    for (int i = 0; i < 4; i++) {
        const float startX = 180.0f;
        const float spacing = 150.0f;
        nameRenderer = TextRenderer(
            glm::vec2(startX + i * spacing, 200),
            new Sprite("Resources/font.png", glm::vec2(24), 1, glm::vec2(15, 8), true),
            20, 32
        );
        nameRenderer.setText(getCharacterName(static_cast<CharacterType>(i)));
        nameRenderer.render();
    }

    // Add instruction text for player 1
    TextRenderer instructionRenderer(
        glm::vec2(150, 150),
        new Sprite("Resources/font.png", glm::vec2(20), 1, glm::vec2(15, 8), true),
        18, 32
    );
    instructionRenderer.setText("USE_A_D_TO_NAVIGATE__ENTER_TO_SELECT");
    instructionRenderer.render();
}

void MainMenuScene::renderPlayer2Selection() {
    if (titleText) {
        titleText->render();
    }
    if (player2Text) {
        player2Text->render();
    }

    // Render character previews
    for (int i = 0; i < characterPreviews.size(); i++) {
        if (characterPreviews[i]) {
            // Dim unavailable characters
            if (!isCharacterAvailable(static_cast<CharacterType>(i)) && currentState == MenuState::PLAYER2_SELECT) {
                glColor3f(0.5f, 0.5f, 0.5f);
            }
            else {
                glColor3f(1.0f, 1.0f, 1.0f);
            }
            characterPreviews[i]->render();
            glColor3f(1.0f, 1.0f, 1.0f); // Reset color
        }
    }

    // Render selector
    if (selector && currentState == MenuState::PLAYER2_SELECT) {
        selector->render();
    }

    // Render player indicators
    if (player1Selected && player1Indicator) {
        player1Indicator->render();
    }
    if (player2Selected && player2Indicator) {
        player2Indicator->render();
    }

    // Render character names
    TextRenderer nameRenderer(
        glm::vec2(100, 200),
        new Sprite("Resources/font.png", glm::vec2(24), 1, glm::vec2(15, 8), true),
        20, 32
    );

    for (int i = 0; i < 4; i++) {
        const float startX = 180.0f;
        const float spacing = 150.0f;
        nameRenderer = TextRenderer(
            glm::vec2(startX + i * spacing, 200),
            new Sprite("Resources/font.png", glm::vec2(24), 1, glm::vec2(15, 8), true),
            20, 32
        );
        nameRenderer.setText(getCharacterName(static_cast<CharacterType>(i)));
        nameRenderer.render();
    }

    // Add instruction text
    if (currentState == MenuState::PLAYER2_SELECT) {
        TextRenderer instructionRenderer(
            glm::vec2(150, 150),
            new Sprite("Resources/font.png", glm::vec2(20), 1, glm::vec2(15, 8), true),
            18, 32
        );
        instructionRenderer.setText("USE_A_D_TO_NAVIGATE__ENTER_TO_SELECT");
        instructionRenderer.render();
    }
    else if (currentState == MenuState::READY_TO_START) {
        TextRenderer readyRenderer(
            glm::vec2(200, 150),
            new Sprite("Resources/font.png", glm::vec2(32), 1, glm::vec2(15, 8), true),
            24, 32
        );
        readyRenderer.setText("STARTING_GAME");
        readyRenderer.render();
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
    // Reset state when entering menu
    currentState = MenuState::MAIN_MENU;
    selectedCharacterIndex = 0;
    player1Selected = false;
    player2Selected = false;
    player1Character = CharacterType::WHITE;
    player2Character = CharacterType::BLACK;
}
