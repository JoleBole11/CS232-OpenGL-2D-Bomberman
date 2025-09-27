#pragma once
#include "Scene.h"
#include "TextRenderer.h"
#include "GameObject.h"
#include "Input.h"
#include "CharacterTypesEnum.h"

class WinScene : public Scene {
private:
    TextRenderer* titleText;
    TextRenderer* winnerText;
    TextRenderer* instructionText;
    GameObject* winnerCharacter;

    // Enhanced draw support
    bool isDraw;
    CharacterType player1CharacterType;
    CharacterType player2CharacterType;
    GameObject* player1Character;
    GameObject* player2Character;

    int winningPlayer;
    CharacterType winnerCharacterType;

    const char* getCharacterSpritePath(CharacterType character);

public:
    WinScene();
    ~WinScene() override;

    void initialize() override;
    void update(float deltaTime) override;
    void render() override;
    void cleanup() override;

    void onEnter() override;
    void setWinner(int playerNumber, CharacterType characterType);
    void setDraw(CharacterType p1Char, CharacterType p2Char);
};