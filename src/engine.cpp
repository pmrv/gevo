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

Cell::Cell(CellGrid* grid, int x, int y) : d_grid(grid), d_x(x), d_y(y)
{
}

Cell::Cell(CellGrid* grid, int x, int y, uint32_t g) : Cell(grid, x, y)
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
    random_shuffle(d_neighbours.begin(), d_neighbours.end());

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
            d_grid->request_revive(n, g);
            break;
        }
    } else
    if (rand() < aggro * RAND_MAX) {
        for (Cell &n : d_neighbours) {
            if (!n.alive() || like(n)) continue;

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

CellGrid::CellGrid(int N) : d_N(N)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            d_cells.push_back(Cell(this, i, j));
        }
    }


    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {

            int l = i - 1, r = i + 1, u = j - 1, d = j + 1;
            if (l < 0) l += N;
            if (u < 0) u += N;
            if (r > N) r -= N;
            if (d > N) d -= N;

            vector<reference_wrapper<Cell>> n;
            n.push_back(ref(d_cells[l + N * j]));
            n.push_back(ref(d_cells[r + N * j]));
            n.push_back(ref(d_cells[i + N * u]));
            n.push_back(ref(d_cells[i + N * d]));

            n.push_back(ref(d_cells[l + N * u]));
            n.push_back(ref(d_cells[r + N * u]));
            n.push_back(ref(d_cells[l + N * d]));
            n.push_back(ref(d_cells[r + N * d]));

            d_cells[i + N * j].neighbours(n);
        }
    }

    // TODO: do that reguarly during steps
    //random_shuffle(d_cells.begin(), d_cells.end());
}

void
CellGrid::foreach(ForEachCell f)
{
    for_each(d_cells.begin(), d_cells.end(), f);
}

void
CellGrid::request_revive(Cell& target, uint32_t genome)
{
    d_revive_queue.push_back({ref(target), genome});
}

void
CellGrid::process_revives()
{
    for (auto req : d_revive_queue) {
        static_cast<Cell&>(req.target).revive(req.genome);
    }
    d_revive_queue.clear();
}
