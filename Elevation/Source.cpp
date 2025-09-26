#include <freeglut.h>
#include "SceneManager.h"
#include "IntroScene.h"
#include "GameScene.h"
#include "GameInstance.h"
#include "Input.h"
#include <memory>

// Global variables for window management
const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 960;
const int GAME_WIDTH = 960;
const int GAME_HEIGHT = 832;
const int WINDOW_POS_X = 50;
const int WINDOW_POS_Y = 50;

int main(int argc, char** argv)
{
    Intro intro(argc, argv, "Bomberman", WINDOW_WIDTH, WINDOW_HEIGHT, 50, 50, GAME_WIDTH, GAME_HEIGHT);
    if (Intro::intro_instance->get_intro_finished())
    {
        Game game(argc, argv, "Bomberman", WINDOW_WIDTH, WINDOW_HEIGHT, 50, 50, GAME_WIDTH, GAME_HEIGHT);
    }
    
    

    return 0;
}
