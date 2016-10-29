// Copyright (C) 2016 Marvin Poul <ponder@creshal.de>

#ifndef GFX_H
#define GFX_H

#include "dbg.h"

#include <iostream>

using namespace std;

#include "SDL.h"
#include "SDL_video.h"
#include "SDL_render.h"

class GfxState {
    private:
        SDL_Window *d_window = NULL;
        SDL_Renderer *d_renderer = NULL;
        int N, d_X, d_Y, DX = d_X / N, DY = d_Y / N;

    public:
        GfxState(int n, int x, int y);
        void draw_rect(int i, int j, uint8_t r, uint8_t g, uint8_t b);
        void prepare();
        void present();

        void X(int x);
        void Y(int y);
        int X();
        int Y();
};

#endif
