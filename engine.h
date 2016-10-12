#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>

#include <functional>
#include <vector>

using namespace std;

class Cell {
    private:
        vector<reference_wrapper<Cell>> d_neighbours;
        bool d_alive;
        unsigned int d_life;
        uint16_t d_genome;

    public:
        Cell();

        void revive(uint16_t g);

        void neighbours(vector<reference_wrapper<Cell>> n);
        vector<reference_wrapper<Cell>> neighbours();

        bool alive();

        void step();

};

typedef void (*ForEachCell)(Cell);

template<int N>
class CellGrid {
    private:
        array<array<Cell, N>, N> d_cells;

    public:
        CellGrid();
        void foreach(ForEachCell f);

};

template<int N>
CellGrid<N>::CellGrid()
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            vector<reference_wrapper<Cell>> n;
            n.push_back(ref(d_cells[(i - 1) % N][j]));
            n.push_back(ref(d_cells[(i + 1) % N][j]));
            n.push_back(ref(d_cells[i][(j - 1) % N]));
            n.push_back(ref(d_cells[i][(j + 1) % N]));
            d_cells[i][j].neighbours(n);
        }
    }
}

template<int N>
void
CellGrid<N>::foreach(ForEachCell f)
{
    // TODO: this introduces bias in favor of the top left cells
    for (auto r : d_cells) {
        for (auto c : r) {
            f(c);
        }
    }
}

#endif
