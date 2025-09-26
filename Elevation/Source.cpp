#include "Game.h"
#include "Intro.h"

int main(int argc, char** argv)
{
    Intro intro(argc, argv, "Bomberman", 960, 832, 50, 50, 960, 832);
    if (Intro::intro_instance->get_intro_finished())
    {
        Game game(argc, argv, "Bomberman", 960, 832, 50, 50, 960, 960);
    }
    
    

    return 0;
}
