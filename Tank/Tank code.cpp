#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "Tank.h"
#include "Map.h"
#include "Game.h"
#include "Wall.h"

using namespace std;

int main(int argc, char* argv[])

{
    Game game;
    if(game.chayCtrinh){
        game.chay();
    }
    return 0;
}
