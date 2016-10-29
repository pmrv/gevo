// Copyright (C) 2016 Marvin Poul <ponder@creshal.de>
#include "engine.h"

uint8_t
bitcount(uint32_t g)
{
    uint8_t c = 0;
    while (g) {
        g &= g - 1;
        c += 1;
    }

    return c;
}

Cell::Cell()
{
}

Cell::Cell(int x, int y) : d_x(x), d_y(y)
{
}

Cell::Cell(int x, int y, uint32_t g) : Cell(x, y)
{
    revive(g);
}

void
Cell::neighbours(vector<reference_wrapper<Cell>> n)
{
    d_neighbours = n;
}

vector<reference_wrapper<Cell>>
Cell::neighbours()
{
    return d_neighbours;
}

void
Cell::die()
{
    d_alive = false;
}

void
Cell::step()
{
    if (d_age <= 0) {
        die();
        return;
    } else {
        d_age -= 1;
    }

    if (rand() < horny * RAND_MAX) {
        for (Cell &n : d_neighbours) {
            if (n.alive()) continue;

            uint32_t g = d_genome;
            if (rand() < mutate * RAND_MAX) {
                g ^= rand() & 0xffffffff;
            }
            n.revive(g);
            break;
        }
    } else
    if (rand() < aggro * RAND_MAX) {
        for (Cell &n : d_neighbours) {
            if (!n.alive()) continue;

            n.attacked(aggro * d_age);
            break;
        }
    }
}

void
Cell::revive(uint32_t g)
{
    d_alive = true;
    d_genome = g;

    d_age  =                    (g & 0x00001f) <<  3;
    hunger =                   ((g & 0x0000e0) >>  5) + 2;
    horny  = static_cast<float>((g & 0x000f00) >>  8) / 32;
    aggro  = static_cast<float>((g & 0x00f000) >> 12) / 32;
    mutate = static_cast<float>((g & 0x1f0000) >> 16) / 32;
}

bool
Cell::like(Cell &c)
{
    return bitcount(~(d_genome ^ c.genome())) < mutate;
}

void
Cell::attacked(float a)
{
    float o = d_age * aggro;
    d_age -= o < a ? a : 0;
}

bool
Cell::alive()
{
    return d_alive;
}

uint32_t
Cell::genome()
{
    return d_genome;
}

uint8_t
Cell::age()
{
    return d_age;
}

int
Cell::x()
{
    return d_x;
}

void
Cell::x(int xx)
{
    d_x = xx;
}

int
Cell::y()
{
    return d_y;
}

void
Cell::y(int yy)
{
    d_y = yy;
}
