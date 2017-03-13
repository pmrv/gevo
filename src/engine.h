// Copyright (C) 2016 Marvin Poul <ponder@creshal.de>

#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>

#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

class CellGrid;

class Cell {
    private:
        CellGrid* d_grid = nullptr;
        vector<reference_wrapper<Cell>> d_neighbours;
        bool d_alive = false;
        uint32_t d_genome = 0xffffffff;
        uint8_t d_age = 0, hunger= 0;
        float horny = 0, aggro = 0, mutate = 0;
        int d_x, d_y;

    public:
        Cell();
        Cell(CellGrid* grid, int x, int y);
        Cell(CellGrid* grid, int x, int y, uint32_t g);

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

struct ReviveRequest {
    reference_wrapper<Cell> target;
    uint32_t genome;
};

class CellGrid {
    private:
        int d_N;
        vector<Cell> d_cells;
        vector<ReviveRequest> d_revive_queue;

    public:
        CellGrid(int N);
        void foreach(ForEachCell f);
        void request_revive(Cell& target, uint32_t genome);
        void process_revives();

};

#endif
