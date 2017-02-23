// Copyright (C) 2016 Marvin Poul <ponder@creshal.de>

#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>

#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

class Cell {
    private:
        vector<reference_wrapper<Cell>> d_neighbours;
        bool d_alive = false;
        uint32_t d_genome = 0xffffffff;
        uint8_t d_age = 0, hunger= 0;
        float horny = 0, aggro = 0, mutate = 0;
        int d_x, d_y;

    public:
        Cell();
        Cell(int x, int y);
        Cell(int x, int y, uint32_t g);

        void revive(uint32_t g);
        void die();
        void step();
        bool like(Cell &c);
        void attacked(float a);

        void neighbours(vector<reference_wrapper<Cell>> n);
        vector<reference_wrapper<Cell>> neighbours();

        uint32_t genome();
        bool alive();
        uint8_t age();
        int x();
        void x(int xx);
        int y();
        void y(int yy);
};

//typedef void (*ForEachCell)(Cell&, void *);
typedef std::function<void(Cell&)> ForEachCell;

template<int N>
class CellGrid {
    private:
        vector<Cell> d_cells;

    public:
        CellGrid();
        void foreach(ForEachCell f);

};

template<int N>
CellGrid<N>::CellGrid()
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            d_cells.push_back(Cell(i, j));
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
            random_shuffle(n.begin(), n.end());
            d_cells[i + N * j].neighbours(n);
        }
    }

    // TODO: do that reguarly during steps
    //random_shuffle(d_cells.begin(), d_cells.end());
}

template<int N>
void
CellGrid<N>::foreach(ForEachCell f)
{
    for_each(d_cells.begin(), d_cells.end(), f);
}

#endif
