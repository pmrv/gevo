// Copyright (C) 2016 Marvin Poul <ponder@creshal.de>

#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>

#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

typedef uint32_t Genome;

class CellGrid;

class Cell {
    private:
        CellGrid* d_grid = nullptr;
        vector<reference_wrapper<Cell>> d_neighbours;
        bool d_alive = false;
        Genome d_genome = 0xffffffff;
        uint8_t d_age = 0, hunger= 0;
        int d_x, d_y;

    public:
        float horny = 0, aggro = 0, mutate = 0;

        Cell();
        Cell(CellGrid* grid, int x, int y);
        Cell(CellGrid* grid, int x, int y, uint32_t g);

        void revive(uint32_t g);
        void die();
        void step();
        bool like(Cell &c);

        void neighbours(vector<reference_wrapper<Cell>> n);
        vector<reference_wrapper<Cell>> neighbours();

        uint32_t genome();
        bool alive();
        uint8_t age();
        float attack();

        int x();
        void x(int xx);
        int y();
        void y(int yy);
};

struct StepStats {
    unordered_map<Genome, int> populus;
};

struct ReviveRequest {
    reference_wrapper<Cell> target;
    uint32_t genome;
    reference_wrapper<Cell> mother;
};

struct DeathRequest {
    reference_wrapper<Cell> target, killer;
};

typedef function<void(Cell&)> ForEachCell;
typedef function<void(StepStats&)> OnStepStats;

class CellGrid {
    private:
        int d_N;
        vector<Cell> d_cells;
        vector<ReviveRequest> d_revive_queue;
        vector<DeathRequest> d_death_queue;
        unordered_map<Genome, int> d_populus;

    public:
        CellGrid(int N);
        void on_live_cells(ForEachCell f);
        void on_step_stats(OnStepStats f);
        void request_revive(Cell& target, uint32_t genome, Cell& mother);
        void request_death(Cell& target, Cell& killer);
        void process_requests();

};

#endif
