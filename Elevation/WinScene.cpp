#include "WinScene.h"
#include "SceneManager.h"
#include <freeglut.h>
#include <iostream>

WinScene::WinScene() : Scene("Win"),
titleText(nullptr),
winnerText(nullptr),
instructionText(nullptr),
winnerCharacter(nullptr),
player1Character(nullptr),
player2Character(nullptr),
winningPlayer(1),
winnerCharacterType(CharacterType::WHITE),
isDraw(false),
player1CharacterType(CharacterType::WHITE),
player2CharacterType(CharacterType::BLACK) {
}

WinScene::~WinScene() {
    cleanup();
}

void WinScene::initialize() {
    titleText = new TextRenderer(
        glm::vec2(300, 700),
        new Sprite("Resources/font.png", glm::vec2(64), 1, glm::vec2(15, 8), true),
        54, 32
    );
    titleText->setText("GAME_OVER");

    winnerText = new TextRenderer(
        glm::vec2(200, 550),
        new Sprite("Resources/font.png", glm::vec2(48), 1, glm::vec2(15, 8), true),
        40, 32
    );

    instructionText = new TextRenderer(
        glm::vec2(250, 200),
        new Sprite("Resources/font.png", glm::vec2(32), 1, glm::vec2(15, 8), true),
        27, 32
    );
    instructionText->setText("PRESS_ENTER_TO_CONTINUE");

    initialized = true;
}

void WinScene::update(float deltaTime) {
    if (Input::getKeyDown(13)) {
        SceneManager::getInstance()->changeScene("MainMenu");
    }
}

void WinScene::render() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (titleText) titleText->render();
    if (winnerText) winnerText->render();
    if (instructionText) instructionText->render();

    if (isDraw) {
        if (player1Character) player1Character->render();
        if (player2Character) player2Character->render();
    }
    else {
        if (winnerCharacter) winnerCharacter->render();
    }
}

void WinScene::cleanup() {
    delete titleText;
    titleText = nullptr;

    delete winnerText;
    winnerText = nullptr;

    delete instructionText;
    instructionText = nullptr;

    delete winnerCharacter;
    winnerCharacter = nullptr;

    delete player1Character;
    player1Character = nullptr;

    delete player2Character;
    player2Character = nullptr;
}

void WinScene::onEnter() {
    if (isDraw) {
        if (winnerText) {
            winnerText->setText("IT_S_A_DRAW");
        }

        if (player1Character) {
            delete player1Character;
            player1Character = nullptr;
        }
        if (player2Character) {
            delete player2Character;
            player2Character = nullptr;
        }

        player1Character = new GameObject(
            glm::vec2(300, 400),
            glm::vec2(0),
            new Sprite(getCharacterSpritePath(player1CharacterType), glm::vec2(128), 1, glm::vec2(4, 3))
        );
        player1Character->get_sprite()->set_current_frame(1);
        player1Character->set_scale(glm::vec2(1.5f, 1.5f));

        player2Character = new GameObject(
            glm::vec2(600, 400),
            glm::vec2(0),
            new Sprite(getCharacterSpritePath(player2CharacterType), glm::vec2(128), 1, glm::vec2(4, 3))
        );
        player2Character->get_sprite()->set_current_frame(1);
        player2Character->set_scale(glm::vec2(1.5f, 1.5f));
    }
    else {
        if (winnerText) {
            std::string winText = (winningPlayer == 1) ? "PLAYER_1_WINS" : "PLAYER_2_WINS";
            winnerText->setText(winText);
        }

        if (winnerCharacter) {
            delete winnerCharacter;
            winnerCharacter = nullptr;
        }

        winnerCharacter = new GameObject(
            glm::vec2(480, 400),
            glm::vec2(0),
            new Sprite(getCharacterSpritePath(winnerCharacterType), glm::vec2(128), 1, glm::vec2(4, 3))
        );
        winnerCharacter->get_sprite()->set_current_frame(1);
        winnerCharacter->set_scale(glm::vec2(2.0f, 2.0f));
    }
}

void WinScene::setWinner(int playerNumber, CharacterType characterType) {
    isDraw = false;
    winningPlayer = playerNumber;
    winnerCharacterType = characterType;

    std::cout << "Winner set: Player " << playerNumber
        << " with character " << static_cast<int>(characterType) << std::endl;
}

void WinScene::setDraw(CharacterType p1Char, CharacterType p2Char) {
    isDraw = true;
    player1CharacterType = p1Char;
    player2CharacterType = p2Char;

    std::cout << "Draw set between Player 1 (" << static_cast<int>(p1Char)
        << ") and Player 2 (" << static_cast<int>(p2Char) << ")" << std::endl;
}

const char* WinScene::getCharacterSpritePath(CharacterType character) {
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