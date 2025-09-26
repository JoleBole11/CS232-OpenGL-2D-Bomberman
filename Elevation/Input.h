#pragma once

#include <stdlib.h>
#include <freeglut.h>
#include <glm.hpp>
#include <iostream>

class Input {
public:
    static bool _isCursorLocked;

private:
    static bool _keyStates[256];
    static bool _keyDownDected[256];

    static glm::vec3 _mousePosition;

    static glm::vec3 _lastMousePosition;
    static glm::vec3 _mouseDeltaPosition;

    static bool _leftMouse;
    static bool _rightMouse;
    static bool _leftMouseDown;
    static bool _rightMouseDown;

public:
    static void setCallbackFunctions();
    static void update();

    static bool getKeyDown(int keycode);
    static bool getKey(int keycode);
    static bool getKeyUp(int keycode);

    static void keyboard(unsigned char key, int x, int y);
    static void keyboardUp(unsigned char key, int x, int y);
    static bool isAnyKeyPressed();

    static glm::vec3 getMousePosition();
    static void mouseClick(int button, int state, int x, int y);
    static glm::vec3 getMouseDeltaPosition();
    static bool getLeftMouseDown();
    static bool getLeftMouse();
    static bool getRightMouseDown();

    static void updateMouse(int x, int y);

private:
    static void updateCursorLock();
};