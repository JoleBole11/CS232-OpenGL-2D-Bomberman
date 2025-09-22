#include "Input.h"

bool Input::_keyStates[256] = { false };
bool Input::_keyDownDected[256] = { false };
bool Input::_isCursorLocked = false;

glm::vec3 Input::_mousePosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 Input::_lastMousePosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 Input::_mouseDeltaPosition = glm::vec3(0.0f, 0.0f, 0.0f);

bool Input::_leftMouse = false;
bool Input::_rightMouse = false;
bool Input::_leftMouseDown = false;
bool Input::_rightMouseDown = false;

void Input::updateMouse(int x, int y)
{
    _mousePosition.x = x;
    _mousePosition.y = y;
}

void Input::setCallbackFunctions() {
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutPassiveMotionFunc(updateMouse);
    glutMouseFunc(mouseClick);
}

bool Input::getKeyDown(int keycode) {
    if (_keyStates[keycode] && !_keyDownDected[keycode]) {
        _keyDownDected[keycode] = true;
        return true;
    }
    return false;
}

bool Input::getKey(int keycode) {
    return _keyStates[keycode];
}

bool Input::getKeyUp(int keycode) {
    if (!_keyStates[keycode] && _keyDownDected[keycode])
        return true;

    return false;
}

void Input::keyboard(unsigned char key, int x, int y) {
    if (isalpha(key))
        key = toupper(key);

    _keyStates[key] = true;
}

void Input::keyboardUp(unsigned char key, int x, int y) {
    if (isalpha(key))
        key = toupper(key);

    _keyStates[key] = false;
    _keyDownDected[key] = false;
}

void Input::mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        _leftMouse = true;
        return;
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        _leftMouse = false;
        _leftMouseDown = false;
        return;
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        _rightMouse = true;
        return;
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
        _rightMouse = false;
        _rightMouseDown = false;
        return;
    }
}

glm::vec3 Input::getMouseDeltaPosition() {
    return _mouseDeltaPosition;
}

bool Input::getLeftMouseDown() {
    if (_leftMouse && !_leftMouseDown) {
        _leftMouseDown = true;
        return true;
    }
    return false;
}

bool Input::getLeftMouse() {
    return _leftMouse;
}

bool Input::getRightMouseDown() {
    if (_rightMouse && !_rightMouseDown) {
        _rightMouseDown = true;
        return true;
    }
    return false;
}

bool Input::isAnyKeyPressed() {
    for (int i = 0; i < 256; ++i) {
        if (_keyStates[i]) {
            return true;
        }
    }
    return false;
}

void Input::update() {
    updateCursorLock();

    if (!_isCursorLocked) {
        _mouseDeltaPosition = _lastMousePosition - _mousePosition;
        _lastMousePosition = _mousePosition;
    }
    else {
        _mouseDeltaPosition.x = glutGet(GLUT_WINDOW_WIDTH) / 2 - _mousePosition.x;
        _mouseDeltaPosition.y = glutGet(GLUT_WINDOW_HEIGHT) / 2 - _mousePosition.y;
    }

    for (int i = 0; i < 256; i++) {
        if (!_keyStates[i]) {
            _keyDownDected[i] = false;
        }
    }
}

void Input::updateCursorLock() {
    if (_isCursorLocked) {
        glutSetCursor(GLUT_CURSOR_NONE);
        glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
    }
    else {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }
}

glm::vec3 Input::getMousePosition() {
    return glm::vec3(_mousePosition.x, (glutGet(GLUT_WINDOW_HEIGHT) - _mousePosition.y), _mousePosition.z);
}