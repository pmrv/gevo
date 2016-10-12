#include "engine.h"
#include "gfx.h"

using namespace std;

int
main(int argc, char **argv)
{
    CellGrid<10> game;
    GfxState gfx;

    bool running = true;
    SDL_Event event;

    while (running) {
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }
    }

    SDL_Quit();
}
