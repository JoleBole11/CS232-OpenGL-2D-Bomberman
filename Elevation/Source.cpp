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
#include "AudioManager.h"

// Global variables for window management
const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 960;
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

void passiveMotionCallback(int x, int y) {
    Input::updateMouse(x, y);
}

void timerCallback(int value) {
    Input::update();
    glutPostRedisplay();
    glutTimerFunc(16, timerCallback, 0);
}

void setupAudio() {
    AudioManager::init();
    AudioManager::addSound("placeBomb", "resources/placeBomb.mp3");
    AudioManager::addSound("explosion", "resources/explosion.mp3");
    AudioManager::addSound("gainPickup", "resources/pickupEffect.mp3");
    AudioManager::addSound("losePickup", "resources/losePickup.mp3");
    AudioManager::addSong("menuSong", "resources/menuSong.mp3");
    AudioManager::addSong("gameSong", "resources/gameSong.mp3");
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
    glutCreateWindow("Bomberman");

    initGL();

	Input::setCallbackFunctions();
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);
    //glutKeyboardFunc(keyboardCallback);
    //glutKeyboardUpFunc(keyboardUpCallback);
    //glutMouseFunc(mouseCallback);
    //glutMotionFunc(motionCallback);
    //glutPassiveMotionFunc(passiveMotionCallback);
    //glutTimerFunc(16, timerCallback, 0);
    setupAudio();

    SceneManager* sceneManager = SceneManager::getInstance();

    sceneManager->addScene("Intro", std::make_unique<IntroScene>());
    sceneManager->addScene("MainMenu", std::make_unique<MainMenuScene>());
    sceneManager->addScene("Game", std::make_unique<GameScene>());
    sceneManager->addScene("Win", std::make_unique<WinScene>());

    sceneManager->changeScene("Intro");

    glutMainLoop();

    return 0;
}