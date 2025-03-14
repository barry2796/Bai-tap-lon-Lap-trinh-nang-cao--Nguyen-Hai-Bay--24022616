#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;
const WIDTH = 800;
const HEIGHT = 600;
const TILESIZE = 40;
const MAPWIDTH = WIDTH/TILESIZE;
const MAPHEIGHT = HEIGHT/TILESIZE;

void render(){
    SDL_SetRenderDrawColor(renderer,128,128,128,255);

    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer,0,0,0,255);

    for()
}

int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
