#include "SceneManager.h"
#include "Scene.h"
#include <freeglut.h>
#include <iostream>

SceneManager* SceneManager::instance = nullptr;

SceneManager* SceneManager::getInstance() {
    if (!instance) {
        instance = new SceneManager();
    }
    return instance;
}

void SceneManager::addScene(const std::string& name, std::unique_ptr<Scene> scene) {
    scenes[name] = std::move(scene);
}

void SceneManager::changeScene(const std::string& name) {
    auto it = scenes.find(name);
    if (it != scenes.end()) {
        nextScene = it->second.get();
        shouldChangeScene = true;
    }
    else {
        std::cerr << "Scene '" << name << "' not found!" << std::endl;
    }
}

void SceneManager::update(float deltaTime) {
    // Handle scene change
    if (shouldChangeScene && nextScene) {
        if (currentScene) {
            currentScene->onExit();
        }

        currentScene = nextScene;
        nextScene = nullptr;
        shouldChangeScene = false;

        if (!currentScene->isInitialized()) {
            currentScene->initialize();
        }
        currentScene->onEnter();
    }

    // Update current scene
    if (currentScene) {
        currentScene->update(deltaTime);
    }
}

void SceneManager::render() {
    if (currentScene) {
        currentScene->render();
    }
}

void SceneManager::handleInput() {
    // Input is handled through callbacks
}

// OpenGL callback functions
void SceneManager::display() {
    static int lastTime = glutGet(GLUT_ELAPSED_TIME);
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    update(deltaTime);
    render();

    glutSwapBuffers();
    glutPostRedisplay();
}

void SceneManager::reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void SceneManager::keyboard(unsigned char key, int x, int y) {
    if (currentScene) {
        currentScene->handleKeyboard(key, x, y);
    }
}

void SceneManager::keyboardUp(unsigned char key, int x, int y) {
    if (currentScene) {
        currentScene->handleKeyboardUp(key, x, y);
    }
}

void SceneManager::mouse(int button, int state, int x, int y) {
    if (currentScene) {
        currentScene->handleMouse(button, state, x, y);
    }
}

void SceneManager::motion(int x, int y) {
    if (currentScene) {
        currentScene->handleMotion(x, y);
    }
}

SceneManager::~SceneManager() {
    scenes.clear();
}