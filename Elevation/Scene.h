#pragma once
#include <string>
#include <vector>
#include "Button.h"

class Scene {
protected:
    std::string name;
    bool initialized;
	std::vector<class Button*> buttons;

public:
    Scene(const std::string& sceneName) : name(sceneName), initialized(false) {}
    virtual ~Scene() = default;

    virtual void initialize() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void cleanup() = 0;

    virtual void onEnter() {}
    virtual void onExit() {}

    const std::string& getName() const { return name; }
    bool isInitialized() const { return initialized; }
};