#pragma once
#include "Scene.h"
#include "TextRenderer.h"
#include "GameObject.h"
#include "Input.h"
#include <vector>

enum class MenuState {
    MAIN_MENU,
    PLAYER1_SELECT,
    PLAYER2_SELECT,
    READY_TO_START
};

enum class CharacterType {
    WHITE = 0,
    BLACK = 1,
    BLUE = 2,
    RED = 3
};

class MainMenuScene : public Scene {
private:
    MenuState currentState;
    TextRenderer* titleText;
    TextRenderer* instructionText;
    TextRenderer* player1Text;
    TextRenderer* player2Text;

    // Character selection
    std::vector<GameObject*> characterPreviews;
    int selectedCharacterIndex;
    CharacterType player1Character;
    CharacterType player2Character;
    bool player1Selected;
    bool player2Selected;

    // Visual indicators
    GameObject* selector;
    GameObject* player1Indicator;
    GameObject* player2Indicator;

    // Input handling
    bool keyPressed;

    void initializeCharacterPreviews();
    void updateMainMenu(float deltaTime);
    void updatePlayer1Selection(float deltaTime);
    void updatePlayer2Selection(float deltaTime);
    void renderMainMenu();
    void renderPlayer1Selection();
    void renderPlayer2Selection();
    void handleMainMenuInput();
    void handlePlayer1SelectionInput();
    void handlePlayer2SelectionInput();
    bool isCharacterAvailable(CharacterType character);
    const char* getCharacterSpritePath(CharacterType character);
    const char* getCharacterName(CharacterType character);

public:
    MainMenuScene();
    ~MainMenuScene() override;

    void initialize() override;
    void update(float deltaTime) override;
    void render() override;
    void cleanup() override;
    void onEnter() override;

    CharacterType getPlayer1Character() const { return player1Character; }
    CharacterType getPlayer2Character() const { return player2Character; }
};