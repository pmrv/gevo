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
        int N, X = 480, Y = 480, DX = X / N, DY = Y / N;

    public:
        GfxState(int n);
        void draw_rect(int i, int j, uint8_t r, uint8_t g, uint8_t b);
        void prepare();
        void present();

};

#endif
