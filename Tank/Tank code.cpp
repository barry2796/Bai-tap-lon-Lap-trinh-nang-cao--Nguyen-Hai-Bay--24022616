
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Tank.h"
#include "Map.h"
#include "Game.h"
#include "Wall.h"
#include <ctime>
#include <cstdlib>
using namespace std;

int main(int argc, char* argv[])

{

    //time để mỗi lần quái sinh ra ở chỗ mới
    srand(time(0));


    Game game;
    if(game.running){
        game.run();
    }
    return 0;
}
