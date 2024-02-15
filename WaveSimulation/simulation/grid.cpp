#include <cassert>

#include "grid.h"

Grid::Grid()
    : dimensions(glm::ivec4(0))
{
}

Grid::Grid(int n0, int n1, int n2, int n3)
{
    resize(n0, n1, n2, n3);
}

void Grid::resize(int n0, int n1, int n2, int n3) 
{
    dimensions = glm::ivec4(n0, n1, n2, n3);
    data.resize(n0 * n1 * n2 * n3);
}

float& Grid::operator()(int i0, int i1, int i2, int i3) 
{
    return data[i3 + dimension(3) * (i2 + dimension(2) * (i1 + dimension(1) * i0))];
}

float const& Grid::operator()(int i0, int i1, int i2, int i3) const 
{
    int idx = i3 + dimension(3) * (i2 + dimension(2) * (i1 + dimension(1) * i0));
    return data[idx];
}

int Grid::dimension(int dim) const 
{ 
    return dimensions[dim]; 
}

