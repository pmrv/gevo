// Copyright (C) 2016 Marvin Poul <ponder@creshal.de>

#include "engine.h"
#include "gfx.h"

#include <fstream>
#include <iostream>
#include <unordered_map>

using namespace std;

template<int N>
struct GfxGame {
    CellGrid<N> grid;
    GfxState gfx = GfxState(N, 480, 480);
};

typedef uint32_t (*CellHash)(Cell &);

#define FPS 60

int
main(int argc, char **argv)
{
    const int M = 40;
    int fps = FPS;
    GfxGame<M> *game = new GfxGame<M>();

    if (argc >= 2) {
        fps = atoi(argv[1]);
        if (fps == 0) {
            cerr << "couldn't parse fps arg\n";
            exit(1);
        }
    }

    bool running = true;
    SDL_Event event;
    unsigned int ticks = 1000 / fps;

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
            [&clades](Cell &c) -> void {
                int x = c.x(), y = c.y();
                if ((x + y) % 3 == 0) c.revive(clades[(x + y) % 10]);
    });

    unordered_map<uint32_t, int> clade_frequency;
    fstream stats("stats", ios_base::out);
    while (running) {
        SDL_Scancode c;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    c = event.key.keysym.scancode;
                    if (c < 30 || c >= 30 + hashes.size()) break;

                    hash_index = event.key.keysym.scancode - 30;
                    break;
                case SDL_WINDOWEVENT:
                    if (event.window.event != SDL_WINDOWEVENT_RESIZED) break;

                    game->gfx.X(event.window.data1);
                    game->gfx.Y(event.window.data2);
                    break;

            }
        }

        clade_frequency.clear();
        hash = hashes[hash_index];
        game->gfx.prepare();
        game->grid.foreach(
                [&game, &hash, &clade_frequency](Cell &c) -> void {
                    if (!c.alive()) return;

                    try {
                        clade_frequency.at(c.genome()) += 1;
                    } catch (out_of_range) {
                        clade_frequency[c.genome()] = 1;
                    }

                    c.step();
                    uint32_t g = hash(c);
                    game->gfx.draw_rect(c.x(), c.y(), (g >> 16) & 0xff,
                                                      (g >>  8) & 0xff,
                                                       g        & 0xff);
                });
        game->gfx.present();

        for (auto &p : clade_frequency) {
            stats << p.first << ' ' << p.second << '\n';
        }
        stats << '\n';

        SDL_Delay(ticks);
    }

    stats.close();
    SDL_Quit();
}
