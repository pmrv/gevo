#include "gfx.h"

GfxState::GfxState()
{ // TODO: proper error handling with exceptions
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(X, Y, 0, &d_window, &d_renderer);
}

void
GfxState::draw_rect(int i, int j, Uint8 r, Uint8 g, Uint8  b)
{
    SDL_SetRenderDrawColor(d_renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_Rect rect = {i * DX, j * DY, DX, DY};
    SDL_RenderDrawRect(d_renderer, &rect);
}
