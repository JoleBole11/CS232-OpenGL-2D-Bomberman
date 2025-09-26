#pragma once
#include <string>

class Scene {
protected:
    std::string name;
    bool initialized;

public:
    Scene(const std::string& sceneName) : name(sceneName), initialized(false) {}
    virtual ~Scene() = default;

    virtual void initialize() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void cleanup() = 0;

    virtual void onEnter() {}
    virtual void onExit() {}

    virtual void handleKeyboard(unsigned char key, int x, int y) {}
    virtual void handleKeyboardUp(unsigned char key, int x, int y) {}
    virtual void handleMouse(int button, int state, int x, int y) {}
    virtual void handleMotion(int x, int y) {}

    const std::string& getName() const { return name; }
    bool isInitialized() const { return initialized; }
};