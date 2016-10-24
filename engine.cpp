#include "engine.h"

Cell::Cell()
{
}

Cell::Cell(uint32_t g) : Cell()
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
Cell::step()
{
    if (rand() < horny * RAND_MAX) {
        for (Cell &n : d_neighbours) {
            if (n.alive()) continue;

            n.revive(d_genome);
            break;
        }
    }
}

void
Cell::revive(uint32_t g)
{
    d_alive = true;
    d_genome = g;
    horny = .25 + static_cast<float>(g & 0xf) / 32;
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
