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
Cell::die()
{
    d_alive = false;
}

void
Cell::step()
{
    if (age <= 0) {
        die();
        return;
    } else {
        age -= 1;
    }

    if (rand() < horny * RAND_MAX) {
        for (Cell &n : d_neighbours) {
            if (n.alive()) continue;

            n.revive(d_genome);
            break;
        }
    } else
    if (rand() < horny * RAND_MAX) {
        for (Cell &n : d_neighbours) {
            if (!n.alive() || d_genome == n.genome()) continue;

            n.attacked(aggro * age);
            break;
        }
    }
}

void
Cell::revive(uint32_t g)
{
    d_alive = true;
    d_genome = g;
    age = ((g & 0xff00) >> 16) + static_cast<uint8_t>(8 * (rand() - 0.5));
    if (age < 10) age += 10;
    horny = static_cast<float>(g & 0x000f) / 32;
    aggro = static_cast<float>(g & 0x00f0 >> 8) / 32;
}

void
Cell::attacked(float a)
{
    float o = age * aggro;
    age -= o < a ? a : 0;
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
