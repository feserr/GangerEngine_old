// Grid.cpp

#include "Grid.h"

#include <cmath>

Grid::Grid(int width, int height, int cellSize) : m_width(width), m_height(height),
    m_cellSize(cellSize)
{
    m_numXcells = ceil((float)m_width / m_cellSize);
    m_numYcells = ceil((float)m_height / m_cellSize);

    // Allocate all the cells
    m_cells.resize(m_numYcells * m_numXcells);
}

Grid::~Grid()
{}

void Grid::AddBall(Ball *ball)
{
    Cell *cell = GetCell(ball->position);
    AddBall(ball, cell);
}

void Grid::AddBall(Ball *ball, Cell *cell)
{
    cell->balls.push_back(ball);
    ball->ownerCell = cell;
    ball->cellVectorIndex = (int)cell->balls.size() - 1;
}

Cell* Grid::GetCell(int x, int y)
{
    if(x < 0) x = 0;
    if(x >= m_numXcells) x = m_numXcells;
    if(y < 0) y = 0;
    if(y >= m_numYcells) y = m_numYcells;

    return &m_cells[y * m_numXcells + x];
}

Cell* Grid::GetCell(const glm::vec2 &pos)
{
    if(pos.x < 0) return &m_cells[0];
    if(pos.x >= m_numXcells) return &m_cells[0];
    if(pos.y < 0) return &m_cells[0];
    if(pos.y >= m_numYcells) return &m_cells[0];

    int cellX = (int)(pos.x / m_cellSize);
    int cellY = (int)(pos.y / m_cellSize);

    return GetCell(cellX, cellY);
}

void Grid::RemoveBallFromCell(Ball *ball)
{
    std::vector<Ball*> &balls = ball->ownerCell->balls;
    // Normal vector swap
    balls[ball->cellVectorIndex] = balls.back();
    balls.pop_back();

    // Update vector index
    if(ball->cellVectorIndex < balls.size())
        balls[ball->cellVectorIndex]->cellVectorIndex = ball->cellVectorIndex;

    // Set the index of ball to 1
    ball->cellVectorIndex = -1;
    ball->ownerCell = nullptr;
}