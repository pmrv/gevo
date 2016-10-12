#include "engine.h"

Cell::Cell()
{
    d_alive = false;
    d_life = 0;
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
Cell::step()
{
}

void
Cell::revive(uint32_t g)
{
    d_alive = true;
    d_genome = g;
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
