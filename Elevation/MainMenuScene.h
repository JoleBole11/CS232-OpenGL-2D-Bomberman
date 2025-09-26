#pragma once
#include "Scene.h"
#include "TextRenderer.h"

class MainMenuScene :
    public Scene
{
private:
    TextRenderer* title;

public:
        MainMenuScene();
        ~MainMenuScene() override;
        void initialize() override;
        void update(float deltaTime) override;
        void render() override;
        void cleanup() override;
		void onEnter() override;
};

