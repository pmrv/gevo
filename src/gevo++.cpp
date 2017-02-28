// Copyright (C) 2016 Marvin Poul <ponder@creshal.de>

#include "engine.h"
#include "gfx.h"

#include <getopt.h>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <thread>

using namespace std;

#define HELP_TEXT "[-f FPS|-h]"

typedef uint32_t (*CellHash)(Cell &);

#define FPS 60

int
main(int argc, char **argv)
{
    char opt;
    const int M = 40;
    int fps = FPS;
    fstream stats("stats", ios_base::out);

    GfxState gfx(M, 480, 480);
    CellGrid current_grid(M);
    CellGrid display_grid;

    while ((opt = getopt(argc, argv, "+hf:")) != -1) {
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
        }
    }

    bool running = true;
    SDL_Event event;

    vector<uint32_t> clades;
    for (int i = 0; i < 10; i++) {
        clades.push_back(static_cast<uint32_t>(rand()));
    }

    size_t hash_index = 0;
    vector<CellHash> hashes;
    hashes.push_back([](Cell &c) -> uint32_t {return c.genome();});
    hashes.push_back([](Cell &c) -> uint32_t {return c.age();});
    CellHash hash = hashes[0];

    current_grid.foreach(
            [&clades](Cell &c) -> void {
                int x = c.x(), y = c.y();
                if ((x + y) % 3 == 0) c.revive(clades[(x + y) % 10]);
    });

    thread stepThread([&current_grid, &running]() -> void {
            while (running) {
                current_grid.foreach([](Cell &c) -> void {
                    if (!c.alive()) return;

                    c.step();
                });
            }
    });

    thread gfxThread([&current_grid, &hash, &gfx, &running](int fps) -> void {
            unsigned int ticks = 1000 / fps;
            while (running) {
                CellGrid grid = current_grid;

                gfx.prepare();
                grid.foreach(
                        [&hash, &gfx](Cell &c) -> void {
                            if (!c.alive()) return;

                            uint32_t g = hash(c);
                            gfx.draw_rect(c.x(), c.y(), (g >> 16) & 0xff,
                                                        (g >>  8) & 0xff,
                                                         g        & 0xff);
                        });
                gfx.present();
                SDL_Delay(ticks);
            }
    }, fps);

    //unordered_map<uint32_t, int> clade_frequency;
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
                    hash = hashes[hash_index];
                    break;
                case SDL_WINDOWEVENT:
                    if (event.window.event != SDL_WINDOWEVENT_RESIZED) break;

                    gfx.X(event.window.data1);
                    gfx.Y(event.window.data2);
                    break;

            }
        }

        //clade_frequency.clear();
        /*
        current_grid.foreach(
                [&gfx, &hash, &clade_frequency](Cell &c) -> void {
                    if (!c.alive()) return;

                    try {
                        clade_frequency.at(c.genome()) += 1;
                    } catch (out_of_range) {
                        clade_frequency[c.genome()] = 1;
                    }
                });
        */


        /*
        for (auto &p : clade_frequency) {
            stats << p.first << ' ' << p.second << '\n';
        }
        stats << '\n';
        */

    }

    stepThread.join();
    gfxThread.join();

    stats.close();
    SDL_Quit();
}
