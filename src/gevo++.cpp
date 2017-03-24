// Copyright (C) 2016 Marvin Poul <ponder@creshal.de>

#include "engine.h"
#include "gfx.h"

#include <getopt.h>
#include <fstream>
#include <iostream>
#include <thread>

using namespace std;

#define HELP_TEXT "[-h | -f FPS | -o FILE | -m MULTIPLIER]"

struct GfxGame {
    public:
        CellGrid grid;
        GfxState gfx;

        GfxGame(int N) : grid(N)
                       , gfx(N, 480, 480) {};
};

typedef uint32_t (*CellHash)(Cell &);

#define FPS 60

int
main(int argc, char **argv)
{
    char opt;
    const int M = 40;
    int fps = FPS;
    int mult = 0;
    string output_file = "stats";

    while ((opt = getopt(argc, argv, "+hf:o:m:")) != -1) {
        switch (opt) {
            case 'h':
            case '?':
                cerr << argv[0] << ' ' << HELP_TEXT << '\n';
                exit(0);

            case 'f':
                fps = atoi(optarg);
                if (fps == 0) {
                    cerr << "couldn't parse fps arg\n";
                    exit(1);
                }
                break;

            case 'o':
                output_file = optarg;
                break;

            case 'm':
                mult = atoi(optarg);
                if (mult == 0) {
                    cerr << "couldn't parse mult arg\n";
                    exit(1);
                }
                // cells steps are run at least once during the sdl pass, so we
                // need to subtract that here
                mult -= 1;
                break;
        }
    }

    bool running = true;
    SDL_Event event;
    GfxGame game = GfxGame(M);

    vector<uint32_t> clades;
    for (int i = 0; i < game.gfx.X() / 3; i++) {
        clades.push_back(static_cast<uint32_t>(rand()));
    }

    size_t hash_index = 0;
    vector<CellHash> hashes = {
        [](Cell& c) -> uint32_t {return c.genome();},
        [](Cell& c) -> uint32_t {return c.age();},
        [](Cell& c) -> uint32_t {return (uint8_t) (256 * c.mutate) <<  8;},
        [](Cell& c) -> uint32_t {return (uint8_t) (512 * c.horny)  << 16;},
        [](Cell& c) -> uint32_t {return (uint8_t) (512 * c.aggro)  <<  0;},
        [](Cell& c) -> uint32_t {return (uint8_t) ( 32 * c.hunger) <<  8;},
    };
    CellHash hash = hashes[0];

    game.grid.on_live_cells(
            [&clades](Cell &c) -> void {
                int x = c.x(), y = c.y();
                if ((x + y) % 3 == 0) c.revive(clades[(x + y) % 10]);
            }
    );

    thread sdlThread(
            [&game, &output_file, &hash, &running, &fps, &mult]() -> void {
                unsigned int ticks = 1000 / fps;

                fstream fstats(output_file, ios_base::out);

                while (running) {
                    for (int i = 0; i < mult; i++) {
                        game.grid.on_live_cells([](Cell &c) -> void {});
                    }

                    game.gfx.prepare();
                    game.grid.on_live_cells(
                            [&game, &hash](Cell &c) -> void {
                                if (!c.alive()) return;

                                uint32_t g = hash(c);
                                game.gfx.draw_rect(c.x(), c.y(), (g >> 16) & 0xff,
                                                                 (g >>  8) & 0xff,
                                                                  g        & 0xff);
                            }
                    );
                    game.gfx.present();

                    game.grid.on_step_stats(
                            [&fstats](StepStats stats) -> void {
                                for (auto &p : stats.populus) {
                                    fstats << p.first << ' ' << p.second << '\n';
                                }
                                fstats << '\n';
                            }
                    );

                    SDL_Delay(ticks);
                }
            }
    );

    while (running) {
        SDL_Scancode c;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_KEYDOWN:
                    c = event.key.keysym.scancode;

                    if (c == SDL_SCANCODE_Q) {
                        running = false;
                    } else {
                        if (c < 30 || c >= 30 + hashes.size()) break;

                        hash_index = event.key.keysym.scancode - 30;
                        hash = hashes[hash_index];
                    }

                    break;

                case SDL_WINDOWEVENT:
                    if (event.window.event != SDL_WINDOWEVENT_RESIZED) break;

                    game.gfx.X(event.window.data1);
                    game.gfx.Y(event.window.data2);
                    break;
            }
        }

        SDL_Delay(1);
    }

    sdlThread.join();
    SDL_Quit();
}
