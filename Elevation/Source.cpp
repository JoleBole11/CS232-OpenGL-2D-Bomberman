#include <freeglut.h>
#include <GL/glu.h>
#include "SceneManager.h"
#include "IntroScene.h"
#include "GameScene.h"
#include "Input.h"
#include <memory>
#include <iostream>
#include "MainMenuScene.h"
#include "WinScene.h"

// Global variables for window management
const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 832;
const int GAME_WIDTH = 960;
const int GAME_HEIGHT = 832;
const int WINDOW_POS_X = 50;
const int WINDOW_POS_Y = 50;

// Initialize OpenGL
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glDisable(GL_DEPTH_TEST);

    // Enable alpha blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// GLUT callback wrapper functions
void displayCallback() {
    SceneManager::getInstance()->display();
}

void reshapeCallback(int width, int height) {
    SceneManager::getInstance()->reshape(width, height);
}

void keyboardCallback(unsigned char key, int x, int y) {
    // Handle input through Input class first
    Input::keyboard(key, x, y);
    // Then pass to scene manager
    SceneManager::getInstance()->keyboard(key, x, y);
}

void keyboardUpCallback(unsigned char key, int x, int y) {
    Input::keyboardUp(key, x, y);
    SceneManager::getInstance()->keyboardUp(key, x, y);
}

void mouseCallback(int button, int state, int x, int y) {
    Input::mouseClick(button, state, x, y);
    SceneManager::getInstance()->mouse(button, state, x, y);
}

void motionCallback(int x, int y) {
    SceneManager::getInstance()->motion(x, y);
}

void passiveMotionCallback(int x, int y) {
    Input::updateMouse(x, y);
}

void timerCallback(int value) {
    Input::update();
    glutPostRedisplay();
    glutTimerFunc(16, timerCallback, 0); // ~60 FPS
}

int main(int argc, char** argv) {
    try {
        // Initialize GLUT
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA);
        glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
        glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
        glutCreateWindow("Bomberman");

        // Initialize OpenGL
        initGL();

        // Set up GLUT callbacks
        glutDisplayFunc(displayCallback);
        glutReshapeFunc(reshapeCallback);
        glutKeyboardFunc(keyboardCallback);
        glutKeyboardUpFunc(keyboardUpCallback);
        glutMouseFunc(mouseCallback);
        glutMotionFunc(motionCallback);
        glutPassiveMotionFunc(passiveMotionCallback);
        glutTimerFunc(16, timerCallback, 0);

        // Initialize scene manager and add scenes
        SceneManager* sceneManager = SceneManager::getInstance();

        // Create and add scenes
        sceneManager->addScene("Intro", std::make_unique<IntroScene>());
        sceneManager->addScene("MainMenu", std::make_unique<MainMenuScene>());
        sceneManager->addScene("Game", std::make_unique<GameScene>());
        sceneManager->addScene("Win", std::make_unique<WinScene>());  // Add this line

        // Start with intro scene
        sceneManager->changeScene("Intro");

        // Start the main loop
        glutMainLoop();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}