#include "engine.h"
#include "gfx.h"

#include <SDL_timer.h>

using namespace std;

template<int N>
struct GfxGame {
    CellGrid<N> grid;
    GfxState gfx = GfxState(N);
};

int
main(int argc, char **argv)
{
    GfxGame<10> game;

    bool running = true;
    SDL_Event event;
    unsigned int ticks = 1000 / 60;

    vector<uint32_t> clades;
    for (int i = 0; i < 10; i++) {
        clades.push_back(static_cast<uint32_t>(rand()));
    }

    game.grid.foreach(
            [](int i, int j, Cell &c, void *user_data) -> void {
                vector<uint32_t> clades = *static_cast<vector<uint32_t> *>(user_data);
                c.revive(clades[(i + j) % 10]);
            }, &clades);

    while (running) {
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }

        game.gfx.prepare();
        game.grid.foreach(
                [](int i, int j, Cell &c, void *user_data) -> void {
                    c.step();
                    GfxState gfx = *static_cast<GfxState *>(user_data);
                    if (c.alive()) {
                        uint32_t g = c.genome();
                        gfx.draw_rect(i, j, (g >> 16) & 0xff,
                                            (g >>  8) & 0xff,
                                             g        & 0xff);
                    }
                }, &game.gfx);
        game.gfx.present();

        SDL_Delay(ticks);
    }

    SDL_Quit();
}
