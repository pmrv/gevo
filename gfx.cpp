#include "gfx.h"

GfxState::GfxState(int n) : N(n)
{ // TODO: proper error handling with exceptions
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(X, Y, 0, &d_window, &d_renderer);
}

void
GfxState::draw_rect(int i, int j, Uint8 r, Uint8 g, Uint8  b)
{
    SDL_SetRenderDrawColor(d_renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_Rect rect = {i * DX, j * DY, DX, DY};
    SDL_RenderFillRect(d_renderer, &rect);
}

void
GfxState::prepare()
{
    SDL_SetRenderDrawColor(d_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(d_renderer);
}

void
GfxState::present()
{
    SDL_RenderPresent(d_renderer);
}
