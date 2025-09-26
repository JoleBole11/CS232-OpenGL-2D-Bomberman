#pragma once
#include "Scene.h"
#include "TextRenderer.h"

class IntroScene : public Scene {
private:
    TextRenderer* introText;
    float duration;
    float timer;

public:
    IntroScene();
    ~IntroScene() override;

    void initialize() override;
    void update(float deltaTime) override;
    void render() override;
    void cleanup() override;

    void onEnter() override;
};