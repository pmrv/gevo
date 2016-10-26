#include "engine.h"
#include "gfx.h"

#include <iostream>

using namespace std;

template<int N>
struct GfxGame {
    CellGrid<N> grid;
    GfxState gfx = GfxState(N);
};

typedef uint32_t (*CellHash)(Cell &);

int
main(int argc, char **argv)
{
    const int M = 40;
    GfxGame<M> *game = new GfxGame<M>();

    bool running = true;
    SDL_Event event;
    unsigned int ticks = 1000 / 60;

    vector<uint32_t> clades;
    for (int i = 0; i < 10; i++) {
        clades.push_back(static_cast<uint32_t>(rand()));
    }

    CellHash hash = NULL;
    size_t hash_index = 0;
    vector<CellHash> hashes;
    hashes.push_back([](Cell &c) -> uint32_t {return c.genome();});
    hashes.push_back([](Cell &c) -> uint32_t {return c.age();});

    game->grid.foreach(
            [&clades](int i, int j, Cell &c) -> void {
                if ((i + j) % 3 == 0) c.revive(clades[(i + j) % 10]);
    });

    while (running) {
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    SDL_Scancode c = event.key.keysym.scancode;
                    if (c < 30 || c > 30 + hashes.size()) break;

                    hash_index = event.key.keysym.scancode - 30;
            }
        }

        hash = hashes[hash_index];
        game->gfx.prepare();
        game->grid.foreach(
                [&game, &hash](int i, int j, Cell &c) -> void {
                    if (c.alive()) {
                        c.step();
                        uint32_t g = hash(c);
                        game->gfx.draw_rect(i, j, (g >> 16) & 0xff,
                                                  (g >>  8) & 0xff,
                                                   g        & 0xff);
                    }
                });
        game->gfx.present();

        SDL_Delay(ticks);
    }

    SDL_Quit();
}
