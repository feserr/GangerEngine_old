// Grid.h

#pragma once

#include <vector>

#include "Ball.h"

struct Cell
{
    std::vector<Ball*> balls;
};

class Grid
{
public:
    Grid(int width, int height, int cellSize);
    ~Grid();

    /// Adds a ball and determines which cell it belongs to
    void AddBall(Ball *ball);
    /// Adds a ball to the specified cell
    void AddBall(Ball *ball, Cell *cell);
    /// Get cell based on cell coordinates
    Cell* GetCell(int x, int y);
    /// Get cell based on window coordinates
    Cell* GetCell(const glm::vec2 &pos);

    void RemoveBallFromCell(Ball *ball);

private:
    std::vector<Cell> m_cells;

    int m_width;
    int m_height;
    int m_cellSize;
    int m_numXcells;
    int m_numYcells;
};
