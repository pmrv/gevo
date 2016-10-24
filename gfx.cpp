#include "gfx.h"

GfxState::GfxState(int n) : N(n)
{ // TODO: proper error handling with exceptions
    int e;
    e = SDL_Init(SDL_INIT_VIDEO);
    check(e == 0);
    e = SDL_CreateWindowAndRenderer(X, Y, 0, &d_window, &d_renderer);
    check(e == 0);

    return;
error:
    cerr << SDL_GetError();
}

void
GfxState::draw_rect(int i, int j, Uint8 r, Uint8 g, Uint8 b)
{
    int e;
    SDL_Rect rect = {i * DX, j * DY, DX, DY};

    e = SDL_SetRenderDrawColor(d_renderer, r, g, b, SDL_ALPHA_OPAQUE);
    check(e == 0);
    e = SDL_RenderFillRect(d_renderer, &rect);
    check(e == 0);

    return;
error:
    cerr << SDL_GetError();
}

void
GfxState::prepare()
{
    int e;

    e = SDL_SetRenderDrawColor(d_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    check(e == 0);
    e = SDL_RenderClear(d_renderer);
    check(e == 0);

    return;
error:
    cerr << SDL_GetError();
}

void
GfxState::present()
{
    SDL_RenderPresent(d_renderer);
}
