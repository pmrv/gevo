// Copyright (C) 2016 Marvin Poul <ponder@creshal.de>

#include "gfx.h"

GfxState::GfxState(int n, int x, int y) : N(n), d_X(x), d_Y(y)
{ // TODO: proper error handling with exceptions
    int e;
    e = SDL_Init(SDL_INIT_VIDEO);
    check(e == 0);
    e = SDL_CreateWindowAndRenderer(d_X, d_Y, 0, &d_window, &d_renderer);
    check(e == 0);

    return;
error:
    cerr << SDL_GetError();
}

void
GfxState::draw_rect(int i, int j, uint8_t r, uint8_t g, uint8_t b)
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

void
GfxState::X(int x)
{
    DX = x / N;
    d_X = DX * N;
    SDL_SetWindowSize(d_window, d_X, d_Y);
}

void
GfxState::Y(int y)
{
    DY = y / N;
    d_Y = DY * N;
    SDL_SetWindowSize(d_window, d_X, d_Y);
}

int
GfxState::X()
{
    return d_X;
}

int
GfxState::Y()
{
    return d_Y;
}
