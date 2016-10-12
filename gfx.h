#ifndef GFX_H
#define GFX_H

using namespace std;

#include "SDL.h"
#include "SDL_video.h"
#include "SDL_render.h"

class GfxState {
    private:
        SDL_Window *d_window;
        SDL_Renderer *d_renderer;
        int N, X = 480, Y = 480, DX = X / N, DY = Y / N;

    public:
        GfxState(int n);
        void draw_rect(int i, int j, Uint8 r, Uint8 g, Uint8 b);
        void prepare();
        void present();

};

#endif
